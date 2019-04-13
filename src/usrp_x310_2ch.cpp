#include "usrp_x310_2ch.h"
#include "globals.h"
#include "lib/bb_lib.h"

namespace po = boost::program_options;

int fft_size_from_rate_and_rbw(float _rate, float _rbw)
{
    float nfft = _rate / _rbw;
    int order = (int)ceil(log2(nfft));
    return pow(2, order);
}

USRP_X310_2CH::USRP_X310_2CH() : isOpen(false)
{
}

bool USRP_X310_2CH::Reconfigure(DFSettings *s)
{
    if (!isOpen)
        return false;

    if (s->RBW() != rbw) {
        if (streaming) {
            StopStreaming();
        }

        rbw = s->RBW();
        iqCount = native_dsp_lut[(int)s->RBWIndex()].fft_size;

//        int order = (int)ceil(log2(iqCount));
//        fftSize = pow(2, order);
        fftSize = native_dsp_lut[(int)s->RBWIndex()].fft_size;
        fftSizeBW = native_dsp_lut[(int)s->RBWIndex()].fft_size_bw;

        // 设置接收机增益
        double gain = s->Gain();
        std::cout << boost::format("Setting RX Gain: %f dB...") % gain << std::endl;
        usrp->set_rx_gain(gain, 0);
        usrp->set_rx_gain(gain, 1);
        std::cout << boost::format("Actual RX Gain: %f dB...") % usrp->get_rx_gain() << std::endl << std::endl;

        // 如果工作在定频模式，设置接收机频率
        if (s->Mode() == WorkMode_FFM) {
            double freq = s->Center();
            double offset = 0.0e6;
            //advanced tuning with tune_request_t uhd::tune_request_t
            uhd::tune_request_t tune_req(freq, offset);
//            tune_req.args = uhd::device_addr_t("mode_n=integer"); //to use Int-N tuning

            //we will tune the frontends in 100ms from now
            uhd::time_spec_t cmd_time = usrp->get_time_now() + uhd::time_spec_t(0.02);
            //sets command time on all devices
            //the next commands are all timed
            usrp->set_command_time(cmd_time);
            //tune channel 0 and channel 1
            usrp->set_rx_freq(tune_req, 0); // Channel 0
            usrp->set_rx_freq(tune_req, 1); // Channel 1
            //end timed commands
            usrp->clear_command_time();

            std::cout << boost::format("Actual RX Freq: %f MHz...") % (usrp->get_rx_freq()/1e6) << std::endl;
        }

        // 设置IQ数据速率
        double rate = native_dsp_lut[(int)s->RBWIndex()].rate;
        //set the rx sample rate (sets across all channels)
        std::cout << boost::format("Setting RX Rate: %f Msps...") % (rate/1e6) << std::endl;
        usrp->set_rx_rate(rate, 0);
        usrp->set_rx_rate(rate, 1);
        std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp->get_rx_rate()/1e6) << std::endl << std::endl;



        StartStreaming();
    }

    return true;
}

bool USRP_X310_2CH::OpenDevice()
{
    //create a usrp device
    std::cout << std::endl;
    std::string args("");
    std::cout << boost::format("Creating the usrp device with: %s...") % args << std::endl;
    usrp = uhd::usrp::multi_usrp::make(args);

    std::string ref("internal");
    usrp->set_clock_source(ref);

    std::string subdev("A:0 A:1");
    usrp->set_rx_subdev_spec(subdev); //sets across all mboards

    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;

    // 设置多通道共本振
//    usrp->set_rx_lo_export_enabled(true,"all",0);   // 通道0的所有本振源为输出状态
    usrp->set_rx_lo_source("internal","all",0);     // 通道0使用内部本振
    usrp->set_rx_lo_source("companion","all",1);     // 通道1使用外部本振（来自通道0输出）
    //    usrp->set_rx_lo_source("external","all",2);
    //    usrp->set_rx_lo_source("external","all",3);

    std::string channel_list("0,1");

    //detect which channels to use
    std::vector<std::string> channel_strings;
    std::vector<size_t> channel_nums;

    boost::split(channel_strings, channel_list, boost::is_any_of("\"',"));
    for(size_t ch = 0; ch < channel_strings.size(); ch++){
        size_t chan = std::stoi(channel_strings[ch]);
        if(chan >= usrp->get_rx_num_channels()){
            throw std::runtime_error("Invalid channel(s) specified.");
        }
        else channel_nums.push_back(std::stoi(channel_strings[ch]));
    }

    //create a receive streamer
    //linearly map channels (index0 = channel0, index1 = channel1, ...)
    uhd::stream_args_t stream_args("fc32", "sc16"); //complex floats
    stream_args.channels = channel_nums;
    rx_stream = usrp->get_rx_stream(stream_args);

    isOpen = true;
    return true;
}

bool USRP_X310_2CH::CloseDevice()
{
    std::cout << std::endl << "Done!" << std::endl << std::endl;
    return true;
}

void USRP_X310_2CH::StartStreaming()
{
    streaming = true;
    thread_handle_fetch = std::thread(&USRP_X310_2CH::FetchRaw, this);
    thread_handle_sort = std::thread(&USRP_X310_2CH::Sort, this);
}

void USRP_X310_2CH::StopStreaming()
{
    if(thread_handle_fetch.joinable()) {
        streaming = false;
        thread_handle_fetch.join();
    }

    if(thread_handle_sort.joinable()) {
        streaming = false;
        thread_handle_sort.join();
    }
}

void USRP_X310_2CH::FetchRaw()
{    
    uhd::set_thread_priority_safe();
    int total_num_samps = fftSize;

    //meta-data will be filled in by recv()
    uhd::rx_metadata_t md;

    const size_t samps_per_buff = rx_stream->get_max_num_samps();
    std::vector<std::vector<std::complex<float> > > buffs(
                usrp->get_rx_num_channels(), std::vector<std::complex<float> >(samps_per_buff)
                );

    //create a vector of pointers to point to each of the channel buffers
    std::vector<std::complex<float> *> buff_ptrs;
    for (size_t i = 0; i < buffs.size(); i++)
        buff_ptrs.push_back(&buffs[i].front());

    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps = total_num_samps;
    stream_cmd.stream_now = false;

    int nchannels = usrp->get_rx_num_channels();
    IQPacket iqpkt(total_num_samps, nchannels);

    const double timeout_fixed = .1;
    size_t count = 0;

    // 根据采样率 rate 和分辨率 rbw 确定数据点数: iqCount = rate/rbw
    // 例如，rate=100Msps, rbw=25kHz, iqCount=100000
    while (streaming) {

        stream_cmd.time_spec = usrp->get_time_now()+uhd::time_spec_t(0.01);
        rx_stream->issue_stream_cmd(stream_cmd);

        double timeout = timeout_fixed; // timeout (delay before receive + padding)
        size_t num_acc_samps = 0;       // number of accumulated samples

        while(num_acc_samps < total_num_samps){
            //receive a single packet
            size_t num_rx_samps = rx_stream->recv(
                        buff_ptrs, samps_per_buff, md, timeout
                        );

            //use a small timeout for subsequent packets
//            timeout = 0.1;

            //handle the error code
            if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT)
                break;
            if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE){
                throw std::runtime_error(str(boost::format(
                                                 "Receiver error %s"
                                                 ) % md.strerror()));
            }

            for (int m = 0; m < buff_ptrs.size(); m++) {
                std::complex<float> *ptr = buff_ptrs.at(m);
                for (int n = 0; n < num_rx_samps; n++) {
                    iqpkt.iqData(num_acc_samps+n, m) = ptr[n];
                }
            }

            num_acc_samps += num_rx_samps;
        }

        if (num_acc_samps != total_num_samps)
            std::cerr << "Receive timeout before all samples received..." << std::endl;
        else {
            iqpkt.center = usrp->get_rx_freq();
            iqpkt.antennaNum = count % NUM_ANTENNAS;
            IQ_Pool.push(iqpkt);
            count++;
        }
    }
}

void USRP_X310_2CH::Sort()
{
    int nchannels = usrp->get_rx_num_channels();
    fvec coe_win(iqCount);
    build_blackman_window(coe_win.memptr(), iqCount);

    cx_fmat addWindow(iqCount, nchannels);

    IQPacket onepacket;
    FFTPacket fftpacket(fftSize, NUM_ANTENNAS);
    IntermediatePacket frame(fftSizeBW, NUM_ANTENNAS);
    uvec bwRangeIndex = regspace<uvec>(fftSize/2-fftSizeBW/2, fftSize/2+fftSizeBW/2-1);
    uvec packet_status(NUM_ANTENNAS, fill::zeros);

    while (streaming) {
        if (IQ_Pool.read_available()) {
            IQ_Pool.pop(onepacket);

            // 对IQ数据作FFT。面临两个问题：一是要利用FFT的高效率，FFT点数必须为2的幂次方；
            // 二是采样时钟可能是任意值，要取得某一分辨率对应的采样点数未必为2的幂次方。
            // 目前采取的方法是：优先保证效率，实际的RBW在要求的RBW最近位置
            for (int m = 0; m < nchannels; m++)
            {
                for (int n = 0; n < iqCount; n++)
                {
                    addWindow(n,m) = onepacket.iqData(n,m) * coe_win(n);
                }
            }

            cx_fmat Y = arma::shift(arma::fft(addWindow, fftSize)/fftSize, fftSize/2);
//            cx_fmat Y = arma::fft(addWindow, fftSize)/fftSize;

            // normalize to channel 1
            fmat A = 20.0*log10(abs(Y));
            fmat rad = arma::arg(Y);
            fvec diff = (rad.col(1)-rad.col(0))*FACTOR_RAD_DEGREE;

            // put data into the correct position
            switch (onepacket.antennaNum) {
            case 0: // a1b1, a2b2
                fftpacket.amplitudes.col(0) = A.col(0);
                fftpacket.amplitudes.col(1) = A.col(1);
                fftpacket.phases.col(0) = diff; // dp21_uncal
                packet_status(0) = 1;
                break;
            case 1: // a1b1, a3b2
                fftpacket.amplitudes.col(0) = A.col(0);
                fftpacket.amplitudes.col(2) = A.col(1);
                fftpacket.phases.col(1) = diff; // dp31_uncal
                packet_status(1) = 1;
                break;
            case 2: // a1b1, a4b2
                fftpacket.amplitudes.col(0) = A.col(0);
                fftpacket.amplitudes.col(3) = A.col(1);
                fftpacket.phases.col(2) = diff; // dp41_uncal
                packet_status(2) = 1;
                break;
            case 3: // a1b1, a5b2
                fftpacket.amplitudes.col(0) = A.col(0);
                fftpacket.amplitudes.col(4) = A.col(1);
                fftpacket.phases.col(3) = diff; // dp51_uncal
                packet_status(3) = 1;
                break;
            case 4: // a2b1, a3b2
                fftpacket.phases.col(4) = diff; // dp32_uncal
                packet_status(4) = 1;
                break;
            default:
                MY_WARNING << "unexpected antenna number!";
                break;
            }

            // if it is full, further process
            if (sum(packet_status) == packet_status.size()) {
                frame.center = onepacket.center;
                frame.amplitudes = fftpacket.amplitudes.rows(bwRangeIndex);

                // 从 fs 带宽数据中取够 bw 带宽数据
                fvec dp23 = fftpacket.phases.col(0)-fftpacket.phases.col(1);
                fvec dp24 = fftpacket.phases.col(0)-fftpacket.phases.col(2);
                fvec dp25 = fftpacket.phases.col(0)-fftpacket.phases.col(3);
                fvec dp21 = fftpacket.phases.col(1)-fftpacket.phases.col(4);
                fvec dp31 = dp21 - dp23;
                fvec dp41 = dp21 - dp24;
                fvec dp51 = dp21 - dp25;

                frame.phases.col(1) = dp21(bwRangeIndex);
                frame.phases.col(2) = dp31(bwRangeIndex);
                frame.phases.col(3) = dp41(bwRangeIndex);
                frame.phases.col(4) = dp51(bwRangeIndex);

                preprocessed_data.push(frame);
//                std::cout << "preprocessed_data coming!" << std::endl;
                packet_status.fill(0);
            }
        }
        else {
            MSleep(1);
        }
    }
}

