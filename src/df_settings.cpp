#include "df_settings.h"

// centers presets corresponding to various bandwidths
const Hzvec centers_ref_bw20M = regspace<Hzvec>(40000000,20000000,3010000000);
const Hzvec centers_ref_bw10M = regspace<Hzvec>(35000000,10000000,3010000000);
const Hzvec centers_ref_bw5M = regspace<Hzvec>(32500000,5000000,3010000000);
const Hzvec centers_ref_bw2p5M = regspace<Hzvec>(31250000, 2500000, 3010000000);

DFSettings::DFSettings() : DFEnabled(false)
{
    wmode = WorkMode_FFM;
    rmode = COMMON;

    center = 500000000;
    user_start = 30000000;
    user_stop = 3000000000;

    // modify rbw and bw within it
//    setRBWGrade(RBWIndex_3p125kHz);
//    setRBWGrade(RBWIndex_6p25kHz);
//    setRBWGrade(RBWIndex_12p5kHz);
    setRBWGrade(RBWIndex_25kHz);

    atten_mode = RFAttenMode_Manual;
    gain = 90;
    atten = 20;
    atten_cal = 0;
    fft_avg_cnt = Avg_Count_8;
    dwell_time = 2;
    sweeping = true;

    calibrating = false;
    calibrator_en = false;
    recv_loop = Loop_Inner;
    calibrating_auto = false;
    ant_switch_auto = true;

    observ = center;
    observIndex = (observ-(center-bw/2))/rbw;
}

DFSettings& DFSettings::operator=(const DFSettings &other)
{
    wmode = other.wmode;
    start = other.start;
    stop = other.stop;
    center = other.center;
    span = other.span;
    user_start = other.user_start;
    user_stop = other.user_stop;
    bw = other.bw;
    bw_index = other.bw_index;
    rbw = other.rbw;
    rbw_index = other.rbw_index;
    atten_mode = other.atten_mode;
    atten = other.atten;
    gain = other.gain;
    fft_avg_cnt = other.fft_avg_cnt;
    atten_cal = other.atten_cal;
    sweeping = other.sweeping;
    dwell_time = other.dwell_time;
    calibrating = other.calibrating;
    calibrator_en = other.calibrator_en;
    recv_loop = other.recv_loop;
    calibrating_auto = other.calibrating_auto;
    ant_switch_auto = other.ant_switch_auto;
    current_ant_layer = other.current_ant_layer;
    DFEnabled = other.DFEnabled;
    observ = other.observ;
    observIndex = other.observIndex;

    // Assuming this is needed for now ?
    emit updated(this);

    return *this;
}

bool DFSettings::operator==(const DFSettings &other) const
{
    if (wmode != other.wmode) return false;

    if (start != other.start) return false;
    if (stop != other.stop) return false;
    if (center != other.center) return false;
    if (span != other.span) return false;
    if (user_start != other.user_start) return false;
    if (user_stop != other.user_stop) return false;
    if (bw != other.bw) return false;
    if (bw_index != other.bw_index) return false;
    if (rbw != other.rbw) return false;
    if (rbw_index != other.rbw_index) return false;
    if (sweeping != other.sweeping) return false;
    if (atten_mode != other.atten_mode) return false;
    if (atten != other.atten) return false;
    if (gain != other.gain) return false;
    if (fft_avg_cnt != other.fft_avg_cnt) return false;
    if (atten_cal != other.atten_cal) return false;
    if (dwell_time != other.dwell_time) return false;
    if (calibrating != other.calibrating) return false;
    if (calibrator_en != other.calibrator_en) return false;
    if (recv_loop != other.recv_loop) return false;
    if (calibrating_auto != other.calibrating_auto) return false;
    if (ant_switch_auto != other.ant_switch_auto) return false;
    if (current_ant_layer != other.current_ant_layer) return false;
    if (DFEnabled != other.DFEnabled) return false;
    if (observ != other.observ) return false;
    if (observIndex != other.observIndex) return false;

    return true;
}

bool DFSettings::operator!=(const DFSettings &other) const
{
    return !(*this == other);
}

// Default values, program launch
void DFSettings::LoadDefaults()
{

}

Hzvec DFSettings::Centers()
{
    Hzvec centers;

    if (wmode == WorkMode_FFM)
    {
        centers.set_size(1);
        centers << center;
    }
    else if (wmode == WorkMode_PSCAN)
    {
        Hzvec centers_ref;

        switch (rbw_index) {
        case RBWIndex_25kHz:
            centers_ref = centers_ref_bw20M;
            break;
        case RBWIndex_12p5kHz:
            centers_ref = centers_ref_bw10M;
            break;
        case RBWIndex_6p25kHz:
            centers_ref = centers_ref_bw5M;
            break;
        case RBWIndex_3p125kHz:
            centers_ref = centers_ref_bw2p5M;
            break;
        default:
            break;
        }

        int ind_start, ind_stop;

        for (int n = 0; n < centers_ref.size(); n++)
        {
            if (user_start >= centers_ref(n)-bw/2 && user_start < centers_ref(n)+bw/2)
            {
                ind_start = n;
            }
            if (user_stop >= centers_ref(n)-bw/2 && user_stop < centers_ref(n)+bw/2)
            {
                ind_stop = n;
            }
        }

        centers = linspace<Hzvec>(centers_ref(ind_start)
                                  ,centers_ref(ind_stop)
                                  ,ind_stop-ind_start+1);
    }

    // left closed, right open
    start = centers.min() - bw/2;
    stop = centers.max() + bw/2 - rbw;
    span = stop - start;

    freqList.set_size(centers.size()*native_dsp_lut[rbw_index].fft_size_bw);

    int count = 0;

    for (int n = 0; n < centers.size(); n++)
    {
        Hz f_start = centers(n) - bw/2;

        for (int m = 0; m < freqList.size(); m++) {
            freqList(count) = f_start + rbw*m;
            count++;
        }
    }

    observ = freqList(observIndex);

    return centers;
}

int DFSettings::NptsRequired()
{
    int npts;

    if (wmode == WorkMode_FFM)
    {
        npts = bw / rbw;
    }
    else if (wmode == WorkMode_PSCAN)
    {
        npts = (user_stop - user_start)/rbw + 1;
    }

    return npts;
}

void DFSettings::setRBWGrade(int _rbw)
{
    rbw_index = (RBWGrade)_rbw;

    switch (rbw_index) {
    case RBWIndex_25kHz:
        rbw = 25000;
        break;
    case RBWIndex_12p5kHz:
        rbw = 12500;
        break;
    case RBWIndex_6p25kHz:
        rbw = 6250;
        break;
    case RBWIndex_3p125kHz:
        rbw = 3125;
        break;
    default:
        break;
    }

    bw = native_dsp_lut[rbw_index].rate;
    updated(this);
}

void DFSettings::setFreqObervIndex(int ind)
{
    observIndex = ind;
    observ = freqList(observIndex);
    updated(this);
}


