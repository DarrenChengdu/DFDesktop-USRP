#ifndef DEVICE_H
#define DEVICE_H

#include "df_settings.h"

enum DeviceType {
    DF_7_channels = 0,
    DF_5_channels
};

class Device
{
public:
    Device(); 

    virtual bool OpenDevice() = 0;
    virtual bool CloseDevice() = 0;
    virtual bool Abort() = 0;
    virtual bool Preset() = 0;
    virtual bool Reconfigure(DFSettings *s) = 0;
    // Returns false on an unfixable error
    virtual bool GetSweep(const DFSettings *s) = 0;

    virtual bool CanPerformSelfTest() const { return false; }


    // Return the timebase that was set or is going to be set
    virtual int SetTimebase(int new_val) = 0;

    bool IsOpen() const { return open; }
    int Handle() const { return id; }

    virtual QString GetDeviceString() const = 0;
    virtual void UpdateDiagnostics() = 0;

    virtual const char* GetLastStatusString() const = 0;

    DeviceType GetDeviceType() const { return device_type; }
    int SerialNumber() const { return serial_number; }
    QString SerialString() const { return serial_string; }
    QString FirmwareString() const { return firmware_string; }

    float LastConfiguredTemp() const { return last_temp; }
    float CurrentTemp() const { return current_temp; }
    float Voltage() const { return voltage; }
    virtual bool IsPowered() const = 0;
    bool ADCOverflow() const { return adc_overflow; }

    int TimebaseReference() const { return timebase_reference; }
    virtual bool NeedsTempCal() const = 0;
    virtual bool StartCalibrating(int typeCAL) = 0;
    virtual bool StopCalibrating() = 0;

protected:
    bool open;
    int id;

    float last_temp; // Temp of last configured state
    float current_temp; // Last retrieved temp
    float voltage;
    float current;

    bool update_diagnostics_string;

    DeviceType device_type;
    int serial_number;
    QString serial_string;
    QString firmware_string;

    int timebase_reference;   // Internal/Ext(AC/DC)
    bool reconfigure_on_next; // set true to reconfigure on next sweep

    bool adc_overflow;

private:


};

#endif // DEVICE_H
