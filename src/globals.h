#ifndef GLOBALS_H
#define GLOBALS_H

#include <unistd.h>
#define MSleep(x) usleep(x*1000)

#include <mutex>
#include <iostream>
using namespace std;
#include <QByteArray>

#include <QDebug>
#include <QObject>
#define MY_INFO     qInfo().noquote()
#define MY_DEBUG    qDebug().noquote()
#define MY_WARNING  qWarning().noquote()
#define MY_CRITICAL qCritical().noquote()
#define MY_FATAL    qFatal().noquote()

#define PI 3.1415926
#define FACTOR_RAD_DEGREE 180.0/PI
#define FACTOR_DEGREE_RAD 1.0/FACTOR_RAD_DEGREE
#define NUM_ANTENNAS 5
#define FFT_LENGTH 8192

#include "armadillo/armadillo"
using namespace arma;

typedef unsigned int Hz;
typedef Col<Hz> Hzvec;
typedef Mat<Hz> Hzmat;
#define DF_MIN_FREQ            30.0e6
#define DF_MAX_FREQ            1350.0e6
#define kHz(x) (Hz(x*1000.0+0.5))
#define MHz(x) (Hz(x*1000000.0+0.5))
#define GHz(x) (Hz(x*1000000000.0+0.5))

// boost
#include "boost/lockfree/spsc_queue.hpp"
using namespace boost::lockfree;

#define IQ_SAVE
#define DEVICE_DEBUG
#define ITU_CHAN_DEBUG

typedef struct DataFrame {
    Hz center;
    Hz bw;
    float gain;
    float amplitudes[NUM_ANTENNAS][FFT_LENGTH];
    float phase_differences[NUM_ANTENNAS][FFT_LENGTH];
} *pDataFrame;

typedef struct FFTPacket {
    fmat amplitudes;
    fmat phases;
    int npts;
    int channelCount;
    Hz center;
    Hz rbw;  // bw = rbw * npts
    float gain;
    int sec;
    int nano;

    FFTPacket () {
        channelCount = NUM_ANTENNAS;
        npts = FFT_LENGTH;
        amplitudes.zeros(npts,channelCount);
        phases.zeros(npts,channelCount);
    }
} FFTPacket;

typedef struct IQPacket {
    cx_fmat iqData;
    int iqCount;
    int channelCount;
    Hz center;
    float rate;  // rbw = rate / iqCount
    float gain;
    int sec;
    int nano;
    int antennaNum;
} IQPacket;

#define SPSC_BUFFER_LENGTH 1024
extern spsc_queue<IQPacket, capacity<SPSC_BUFFER_LENGTH>> IQ_Pool;
extern spsc_queue<FFTPacket, capacity<SPSC_BUFFER_LENGTH>> FFT_Pool;
extern spsc_queue< DataFrame, capacity<SPSC_BUFFER_LENGTH> > data_frames_raw;

extern spsc_queue< mat, capacity<SPSC_BUFFER_LENGTH> > phases_q;
extern spsc_queue< mat, capacity<SPSC_BUFFER_LENGTH> > amplitudes_q;
extern std::mutex snapshot_mtx;
extern std::mutex raw_mtx;

extern std::mutex settingsMutex;

// calibration about IQ
const unsigned int amp_bw200k_0dBm = 1430000;

const float amp_bw200k_0dBm_recip = 1.0 / amp_bw200k_0dBm;
extern float getCalibratedIQ(int val, int gainRF);
extern float get_rssi_fromIQ(unsigned int amplitude, int gainRF);
extern float get_rssidBm_fromIQ(unsigned int amplitude, int gainRF);

template<typename T>
T getValueFromQByteArray(QByteArray &ba, int offset)
{
    if (ba.length() <= offset || offset+sizeof(T) > ba.length()) {
        cout << "getValueFromQByteArray warning!" << endl;
        cout << "Length of QByteArray: " << ba.length() << "; Bytes offset: "  << offset << endl;
        return 0;
    }

    return *(reinterpret_cast<T *>(&(ba.data()[offset])));
}

enum WorkMode
{
    WorkMode_FFM = 0,
    WorkMode_PSCAN
};

enum ReceiverLoop
{
    Loop_Outer = 0,
    Loop_Inner
};

enum ReceiverMode {
    COMMON = 0,
    LOW_NOISE = 1,
    LARGE_DYNAMIC
};

enum RFAttenMode
{
    RFAttenMode_Manual = 0,
    RFAttenMode_Auto
};

enum FFTAvgCnt
{
    Avg_Count_8 = 0,
    Avg_Count_16 = 1,
    Avg_Count_32
};

enum Bandwidths
{
    Bandwidths_20MHz = 0,
    Bandwidths_10MHz = 1,
    Bandwidths_5MHz = 2,
    Bandwidths_2p5MHz
};

enum RBWGrade
{
    RBWIndex_25kHz = 0,
    RBWIndex_12p5kHz = 1,
    RBWIndex_6p25kHz = 2,
    RBWIndex_3p125kHz
};

#endif // GLOBALS

