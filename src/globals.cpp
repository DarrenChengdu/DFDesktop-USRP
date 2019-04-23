#include "globals.h"
#include "src/lib/bb_lib.h"

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
    { 25000000, 20000000, 25000, 1000,  800 },  //0
    { 25000000, 20000000, 12500, 2000, 1600 },  //1
    { 25000000, 20000000, 6250,  4000, 3200 },  //2
    { 25000000, 20000000, 3125,  8000, 6400 },   //3
    { 12500000, 10000000, 25000,  500,  400 },  //0
    { 12500000, 10000000, 12500, 1000,  800 },  //1
    { 12500000, 10000000,  6250, 2000, 1600 },  //2
    { 12500000, 10000000,  3125, 4000, 3200 },   //3
    {  6250000,  5000000, 25000,  250,  200 },  //0
    {  6250000,  5000000, 12500,  500,  400 },  //1
    {  6250000,  5000000, 6250,  1000,  800 },  //2
    {  6250000,  5000000, 3125,  2000, 1600 }   //3
};

unsigned int GetDSPLUTIndex(int bwIndex, int rbwIndex)
{
    unsigned int lutIndex = bwIndex*4 + rbwIndex;

    if (lutIndex > 11) {
        MY_WARNING << "lutIndex > 11";
        bb_lib::clamp(lutIndex, (unsigned int)0, (unsigned int)11);
    }

    return lutIndex;
}

//dsp_params_lut native_dsp_lut[] = {
//    { 5000000, 4000000, 25000, 200, 160 },  //0
//    { 5000000, 4000000, 12500, 400, 320 },  //1
//    { 5000000, 4000000, 6250,  800, 640 },  //2
//    { 5000000, 4000000, 3125, 1600, 1280 }   //3
//};
