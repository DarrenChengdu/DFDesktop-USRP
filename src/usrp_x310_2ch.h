#ifndef USRP_X310_2CH_H
#define USRP_X310_2CH_H

#include "device.h"
#include <thread>
#include <atomic>
#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

class USRP_X310_2CH : public Device
{    
public:
    USRP_X310_2CH();

    virtual bool OpenDevice();
    virtual bool IsOpen() const;
    virtual bool CloseDevice();
    virtual void StartStreaming();
    virtual void StopStreaming();
    virtual bool Reconfigure(DFSettings *s);

private:
    bool isOpen;

    void FetchRaw();
    void Sort();
    std::atomic<bool> streaming;
    std::thread thread_handle_fetch;
    std::thread thread_handle_sort;

    Hz rbw;
    int iqCount;
    int fftSize;
    int fftSizeBW;

    uhd::usrp::multi_usrp::sptr usrp;
    uhd::rx_streamer::sptr rx_stream;
};

#endif // USRP_X310_2CH_H
