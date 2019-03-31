#ifndef DATA_FACTORY_H
#define DATA_FACTORY_H

#include "globals.h"
#include "session.h"
#include <atomic>

#include "armadillo/armadillo"
using namespace arma;

class DataFactory
{
public:
    DataFactory(Session *sPtr);
    void setDFEnabled(bool enabled);
    bool isDFEnabled() const {return DFEnabled;}
    void Init(int _nchannels, int _npts, Hz _rbw, Hzvec centers);
    void Init(int _nchannels, int _npts, Hz _rbw, Hz _start, Hz _stop, Hz _step);

    Hz FreqCenterMin() const {return min(freqs_center);}
    Hz FreqCenterMax() const {return max(freqs_center);}
    Hzvec Centers() const {return freqs_center;}
    int Length() const {return length;}
    int Npts_Single_Band() const {return npts;}
    bool IsInitialized() const {return initialized;}
    bool Push(const DataFrame &frame);
    bool IsCalibrating() const {return calibrating;}
    void SwitchToCAL(bool sta);
    bool AllDone();
    Hz RBW() const {return rbw;}
    Hz BW() const {return bw;}

    fvec atten_rf;
    fmat amplitudes;
    fmat amplitudes_cal;
    fmat phases_cal;
    fmat phases;
    fvec bearings;
    fmat corrCurve;

    fmat phases_cal_last;
    uvec calCount;

private:

    Session *session_ptr;

    Hzvec freqs_center;
    vec time_stamps;

    int nchannels;
    int npts;
    int length;
    uvec bands_status;
    bool initialized;
    std::atomic<bool> calibrating;
    Hz rbw;
    Hz bw;
    bool DFEnabled;

signals:

};

#endif // DATA_FACTORY_H
