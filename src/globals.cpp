#include "globals.h"

std::mutex settingsMutex;

spsc_queue< DataFrame, capacity<SPSC_BUFFER_LENGTH> > data_frames_raw;
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
