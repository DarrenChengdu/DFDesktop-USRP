#include "seven_channels.h"
#include <QFile>

SevenChannels::SevenChannels()
{

    OpenDevice();
}

bool SevenChannels::Reconfigure(DFSettings *s)
{


    return true;
}

bool SevenChannels::StartCalibrating(int _type)
{


    return true;
}

bool SevenChannels::StopCalibrating()
{


    return true;
}

bool SevenChannels::OpenDevice()
{


    return true;
}

bool SevenChannels::CloseDevice()
{

    return true;
}
