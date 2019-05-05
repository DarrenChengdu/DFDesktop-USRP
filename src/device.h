#ifndef DEVICE_H
#define DEVICE_H

#include "df_settings.h"

class Device
{
public:
    Device(); 

    virtual bool OpenDevice() = 0;
    virtual bool IsOpen() const = 0;
    virtual bool CloseDevice() = 0;
    virtual void StartStreaming() = 0;
    virtual void StopStreaming() = 0;
    virtual bool Reconfigure(DFSettings *s) = 0;

protected:


private:


};

#endif // DEVICE_H
