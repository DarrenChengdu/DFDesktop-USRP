#ifndef USRP_X310_2CH_H
#define USRP_X310_2CH_H

#include "device.h"
#include <thread>
#include <atomic>

class USRP_X310_2CH : public Device
{    
public:
    USRP_X310_2CH();

    virtual bool OpenDevice();
    virtual bool CloseDevice();
    virtual void StartStreaming();
    virtual void StopStreaming();
    virtual bool Reconfigure(DFSettings *s);

private:
    void FetchRaw();
    void Sort();
    std::atomic<bool> streaming;
    std::thread thread_handle_fetch;
    std::thread thread_handle_sort;

    Hz rbw;
    int iqCount;
    int fftSizeBW;
    int fftSize;
};

#endif // USRP_X310_2CH_H
