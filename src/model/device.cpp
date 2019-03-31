#ifndef DEVICE_CPP
#define DEVICE_CPP

#include "device.h"
#include "lib/jq_api.h"
#include "lib/sa_api.h"

QList<DeviceConnectionInfo> Device::GetDeviceList() const
{
    QList<DeviceConnectionInfo> deviceList;
    DeviceConnectionInfo info;

    int serialNumbers[8];
    int deviceCount;

    info.series = saSeries;
    saGetSerialNumberList(serialNumbers, &deviceCount);
    for(int i = 0; i < deviceCount; i++) {
        info.serialNumber = serialNumbers[i];
        deviceList.push_back(info);
    }

    info.series = bbSeries;
	deviceCount = 1;
	serialNumbers[0] = 10086;
    //GetSerialNumberList(serialNumbers, &deviceCount);
    for(int i = 0; i < deviceCount; i++) {
        info.serialNumber = serialNumbers[i];
        deviceList.push_back(info);
    }

    return deviceList;
}

#endif // DEVICE_CPP
