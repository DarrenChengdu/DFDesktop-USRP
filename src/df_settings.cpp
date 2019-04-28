#include "df_settings.h"
#include "lib/bb_lib.h"

// centers presets corresponding to various bandwidths
const Hzvec centers_ref_bw20M = regspace<Hzvec>(40000000,20000000,3010000000);
const Hzvec centers_ref_bw10M = regspace<Hzvec>(35000000,10000000,3010000000);
const Hzvec centers_ref_bw5M = regspace<Hzvec>(32500000,5000000,3010000000);
const Hzvec centers_ref_bw2p5M = regspace<Hzvec>(31250000, 2500000, 3010000000);

DFSettings::DFSettings() : DFEnabled(false), bw_index(Bandwidths_20MHz), rbw_index(RBWIndex_25kHz)
{
    wmode = WorkMode_FFM;
    rmode = COMMON;

    center = 500000000;
    user_start = 30000000;
    user_stop = 3000000000;

    // FFM
    centers.set_size(1);
    centers(0) = center;

    // modify rbw and bw within it
//    setRBWGrade(RBWIndex_3p125kHz);
//    setRBWGrade(RBWIndex_6p25kHz);
//    setRBWGrade(RBWIndex_12p5kHz);
    setRBWGrade(RBWIndex_25kHz);
    setBandwidth(Bandwidths_20MHz);
    setObservation(center);

    atten_mode = RFAttenMode_Manual;
    gain = 40;
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
    freq_observ = other.freq_observ;
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
    if (freq_observ != other.freq_observ) return false;
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
    return centers;
}

void DFSettings::generateFrequencyList()
{
    freqList.set_size(centers.size()*native_dsp_lut[GetDSPLUTIndex(bw_index,rbw_index)].fft_size_bw);

    int count = 0;

    for (int n = 0; n < centers.size(); n++)
    {
        Hz f_start = centers(n) - bw/2;

        for (int m = 0; m < freqList.size(); m++) {
            freqList(count) = f_start + rbw*m;
            count++;
        }
    }
}

void DFSettings::setCenter(Frequency _center){
    center = _center.Val();

    if (wmode == WorkMode_FFM)
    {
        centers.set_size(1);
        centers(0) = center;

        start = centers.min() - bw/2;
        stop = centers.max() + bw/2 - rbw;
        span = stop - start;
    }

    updated(this);
}

void DFSettings::setRBWGrade(int _rbw)
{
    rbw_index = (RBWGrade)_rbw;
    rbw = native_dsp_lut[GetDSPLUTIndex(bw_index,rbw_index)].rbw;

    generateFrequencyList();
    setObservation(freq_observ);
    updated(this);
}

void DFSettings::setGain(double _gain)
{
    gain = _gain;
    bb_lib::clamp(gain, (unsigned int)0, (unsigned int)95);
    updated(this);
}

void DFSettings::setBandwidth(int _bw)
{
    bw_index = (Bandwidths)_bw;
    bw = native_dsp_lut[GetDSPLUTIndex(bw_index,rbw_index)].bw;

    generateFrequencyList();
    setObservation(freq_observ);
    updated(this);
}

void DFSettings::setObservation(Frequency f)
{
    if (f < freqList.min()) {
        observIndex = 0;
    }
    else if (f > freqList.max()) {
        observIndex = freqList.size()-1;
    }
    else {
        Hzvec distance = abs(freqList-f.Val());
        observIndex = distance.index_min();
    }

    freq_observ = freqList(observIndex);
    updated(this);
}

int DFSettings::ObservIndex() const {
    return observIndex;
}


