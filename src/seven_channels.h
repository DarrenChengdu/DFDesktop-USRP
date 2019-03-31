#ifndef SEVEN_CHANNELS_H
#define SEVEN_CHANNELS_H

#include "device.h"

class SevenChannels : public Device
{    
public:
    SevenChannels();

    virtual bool OpenDevice();    
    virtual bool CloseDevice();
    virtual bool Abort() {return true;}
    virtual bool Preset() {return true;}
    virtual bool Reconfigure(DFSettings *s);
    // Returns false on an unfixable error
    virtual bool GetSweep(const DFSettings *s) {return true;}
    virtual bool CanPerformSelfTest() const { return false; }

    // Return the timebase that was set or is going to be set
    virtual int SetTimebase(int new_val) {return 1;}
    virtual QString GetDeviceString() const {return QString("");}
    virtual void UpdateDiagnostics() {}
    virtual const char* GetLastStatusString() const {return NULL;}

    virtual bool IsPowered() const {return true;}
    virtual bool NeedsTempCal() const {return true;}
    virtual bool StartCalibrating(int typeCAL);
    virtual bool StopCalibrating();

private:
//    bool WideDDCCallBack(void* pContext,unsigned long size);
};

#endif // SEVEN_CHANNELS_H
