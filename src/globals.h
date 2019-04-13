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
#define NUM_CHANNELS 2

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

struct dsp_params_lut { size_t rate, bw, rbw, fft_size, fft_size_bw; };
extern dsp_params_lut native_dsp_lut[];

typedef struct IntermediatePacket {
    Hz center;
    Hz bw;
    float gain;
    int npts;
    int nants;
    fmat amplitudes;
    fmat phases;
    IntermediatePacket(int nfft, int nAnts) {
        npts = nfft;
        nants = nAnts;
        amplitudes.zeros(npts,nants);
        phases.zeros(npts,nants);
    }
    IntermediatePacket(){}
} IntermediatePacket;

typedef struct FFTPacket {
    fmat amplitudes;
    fmat phases;
    int npts;
    int ncols;
    Hz center;
    Hz rbw;  // bw = rbw * npts
    float gain;
    int sec;
    int nano;

    FFTPacket (int  _nrows, int _ncols) {
        ncols = _ncols;
        npts = _nrows;
        amplitudes.zeros(npts,ncols);
        phases.zeros(npts,ncols);
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
    IQPacket(int num_iqs, int num_chans) {
        iqCount = num_iqs;
        channelCount = num_chans;
        iqData.zeros(iqCount, channelCount);
    }

    IQPacket(){}
} IQPacket;

#define SPSC_BUFFER_LENGTH 1024
extern spsc_queue<IQPacket, capacity<SPSC_BUFFER_LENGTH>> IQ_Pool;
extern spsc_queue<IntermediatePacket, capacity<SPSC_BUFFER_LENGTH>> preprocessed_data;

extern spsc_queue<mat, capacity<SPSC_BUFFER_LENGTH>> phases_q;
extern spsc_queue<mat, capacity<SPSC_BUFFER_LENGTH>> amplitudes_q;
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

