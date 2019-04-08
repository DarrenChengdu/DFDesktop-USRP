#include "usrp_x310_2ch.h"
#include "globals.h"

int fft_size_from_rate_and_rbw(float _rate, float _rbw)
{
    float nfft = _rate / _rbw;
    int order = (int)ceil(log2(nfft));
    return pow(2, order);
}

USRP_X310_2CH::USRP_X310_2CH()
{
}

bool USRP_X310_2CH::Reconfigure(DFSettings *s)
{
    if (s->RBW() != rbw) {
        if (streaming) {
            StopStreaming();
        }

        rbw = s->RBW();
        iqCount = native_dsp_lut[(int)s->RBWIndex()].fft_size;

        int order = (int)ceil(log2(iqCount));
        fftSize = pow(2, order);
        fftSizeBW = native_dsp_lut[(int)s->RBWIndex()].fft_size_bw;
        StartStreaming();
    }

    return true;
}

bool USRP_X310_2CH::OpenDevice()
{


    return true;
}

bool USRP_X310_2CH::CloseDevice()
{

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
    // 根据采样率 rate 和分辨率 rbw 确定数据点数: iqCount = rate/rbw
    // 例如，rate=100Msps, rbw=25kHz, iqCount=100000
    while (streaming) {

    }
}

void USRP_X310_2CH::Sort()
{
    IQPacket onepacket;
    FFTPacket fftpacket(fftSize, NUM_ANTENNAS);
    IntermediatePacket frame(fftSizeBW, NUM_ANTENNAS);
    uvec bwRangeIdx = regspace<uvec>(fftSize/2-fftSizeBW/2,fftSize/2+fftSizeBW/2-1);
    uvec packet_status(fftpacket.ncols,fill::zeros);

    while (streaming) {
        if (IQ_Pool.read_available()) {
            IQ_Pool.pop(onepacket);

            // 对IQ数据作FFT。面临两个问题：一是要利用FFT的高效率，FFT点数必须为2的幂次方；
            // 二是采样时钟可能是任意值，要取得某一分辨率对应的采样点数未必为2的幂次方。
            // 目前采取的方法是：优先保证效率，实际的RBW在要求的RBW最近位置
            cx_fmat Y = arma::shift(arma::fft(onepacket.iqData, fftSize), fftSize/2);

            // normalize to channel 1
            fmat A = 20.0*log10(abs(Y))/fftSize;
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
                // 从 fs 带宽数据中取够 bw 带宽数据
                fvec dp23 = fftpacket.phases.col(0)-fftpacket.phases.col(1);
                fvec dp24 = fftpacket.phases.col(0)-fftpacket.phases.col(2);
                fvec dp25 = fftpacket.phases.col(0)-fftpacket.phases.col(3);
                fvec dp21 = fftpacket.phases.col(1)-fftpacket.phases.col(4);
                fvec dp31 = dp21 - dp23;
                fvec dp41 = dp21 - dp24;
                fvec dp51 = dp21 - dp25;

                frame.phase_differences.col(1) = dp21(bwRangeIdx);
                frame.phase_differences.col(2) = dp31(bwRangeIdx);
                frame.phase_differences.col(3) = dp41(bwRangeIdx);
                frame.phase_differences.col(4) = dp51(bwRangeIdx);

                preprocessed_data.push(frame);
                packet_status.fill(0);
            }
        }
        else {
            MSleep(1);
        }
    }
}

