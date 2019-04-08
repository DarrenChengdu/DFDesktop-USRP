#include "globals.h"

std::mutex settingsMutex;

spsc_queue<IQPacket, capacity<SPSC_BUFFER_LENGTH>> IQ_Pool;
spsc_queue<IntermediatePacket, capacity<SPSC_BUFFER_LENGTH>> preprocessed_data;
spsc_queue< mat, capacity<SPSC_BUFFER_LENGTH> > phases_q;
spsc_queue< mat, capacity<SPSC_BUFFER_LENGTH> > amplitudes_q;
std::mutex snapshot_mtx;
std::mutex raw_mtx;

float getCalibratedIQ(int val, int gainRF) {
    float val_hat = val * powf(10.0f, -gainRF/20.0f);
    return val_hat * amp_bw200k_0dBm_recip;
}

float get_rssi_fromIQ(unsigned int amplitude, int gainRF) {
    float real_amp = amplitude * powf(10.0f, -gainRF/20.0f);
    return real_amp * amp_bw200k_0dBm_recip;
}

float get_rssidBm_fromIQ(unsigned int amplitude, int gainRF) {
    return 20.0*log10(get_rssi_fromIQ(amplitude, gainRF));
}

//dsp_params_lut native_dsp_lut[] = {
//    { 100.0e6, 80.0e6, 25000.0, 4000 },  //0
//    { 100.0e6, 80.0e6, 12500.0, 8000 },  //1
//    { 100.0e6, 80.0e6, 6250.0, 16000 },  //2
//    { 100.0e6, 80.0e6, 3125.0, 32000 }   //3
//};

dsp_params_lut native_dsp_lut[] = {
    { 25000000, 20000000, 25000, 1000, 800 },  //0
    { 25000000, 20000000, 12500, 2000, 1600 },  //1
    { 25000000, 20000000, 6250, 4000, 3200 },  //2
    { 25000000, 20000000, 3125, 8000, 6400 }   //3
};
