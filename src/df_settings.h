#ifndef _DF_SETTINGS_H
#define _DF_SETTINGS_H

#include <QObject>
#include <atomic>
#include "globals.h"
#include "lib/frequency.h"

class QSettings;
typedef unsigned int Time_ms;

class DFSettings : public QObject {
	Q_OBJECT

public:

    DFSettings();
	DFSettings(const DFSettings &other){ *this = other; }
	~DFSettings() {}

	DFSettings& operator=(const DFSettings &other);
	bool operator==(const DFSettings &other) const;
	bool operator!=(const DFSettings &other) const;
	bool Match(const DFSettings &other);

	void LoadDefaults();
	bool Load(QSettings &s){}
	bool Save(QSettings &s) const{}

    WorkMode Mode() const { return wmode; }
    Hzvec Centers();

	// Freq getters
    Hz Start() const { return start; }
    Hz Stop() const { return stop; }
    Hz UserStart() const { return user_start; }
    Hz UserStop() const { return user_stop; }
    Hz Center() const { return center; }
    Hz Span() const { return span; }
    RBWGrade RBWIndex() const { return rbw_index; }
    Hz RBW() const { return rbw; }
    Hz BW() const { return bw; }
    Bandwidths BWIndex() const { return bw_index; }
    unsigned int DwellTime() const {return dwell_time;}

    unsigned int dBAttenCAL() const { return atten_cal; }
    unsigned int dBAttenRF() const { return atten; }
    ReceiverMode RecvMode() const {return rmode;}
    RFAttenMode AttenModeRF() const { return atten_mode; }
    FFTAvgCnt AvgCount() const {return fft_avg_cnt;}

    bool isSweeping() const { return sweeping; }
    int CurrentAntLayer() const {return current_ant_layer;}
    bool AntSwitchAuto() const {return ant_switch_auto;}
    bool CalibratorEnabled() const {return calibrator_en;}
    int TypeCAL() const {return (int)recv_loop;}
    bool isCalibrating() const {return calibrating;}
    bool isCalibratingAuto() const {return calibrating_auto;}
    bool isDFEnabled() const {return DFEnabled;}
    Hz FreqObserv() const {return observ;}
    int FreqObservIndex() const {return observIndex;}
    Hz FreqMin() {return freqList.min();}
    Hz FreqMax() {return freqList.max();}
    Hzvec FreqList() const {return freqList;}

protected:

private:
    WorkMode wmode;
    ReceiverMode rmode;

    Hz user_start;
    Hz user_stop;
    Hz start;
    Hz stop;
    Hz center;
    Hz span;

    Hz bw;
    Bandwidths bw_index;
    Hz rbw;
    RBWGrade rbw_index;
    RFAttenMode atten_mode;
    unsigned int atten;
    unsigned int atten_cal;
    FFTAvgCnt fft_avg_cnt;
    unsigned int dwell_time;
    bool calibrator_en;
    bool calibrating;
    bool calibrating_auto;
    ReceiverLoop recv_loop;
    bool ant_switch_auto;
    int current_ant_layer;
    bool DFEnabled;
    Hz observ;
    int observIndex;
    Hzvec freqList;

	bool sweeping;

    // compass
    QString serialPortName;
    int serialPortBaudRate;
    int compassRefreshTime_ms;

    // antennas
    int nLayers;
    QList<Frequency> boundaryFreqs;

public slots:
	// Official setters manipulated from control panel
	// Each slot setter must emit updated() only once
	//   at the end of the function regardless of whether
	//   values were updated or not.
    void setAntLayer(int ind) { current_ant_layer = ind;  updated(this);}
    void setAntSwitchAuto(bool sta) { ant_switch_auto = sta; updated(this);}
    void setCalibration(int _type) { recv_loop = (ReceiverLoop)_type; updated(this);}
    void setCalibratorEnabled(bool sta) { calibrator_en = sta; updated(this); }
    void setCalibratingAuto(bool sta) { calibrating_auto = sta; updated(this); }
    void setFFTAvgCnt(int n) { fft_avg_cnt = (FFTAvgCnt)n;  updated(this);}
    void setRecvMode(int _rmode){ rmode = (ReceiverMode)_rmode;  updated(this);}
    void setWorkMode(WorkMode _mode){ wmode = _mode;  updated(this);}
    void setUserStart(Frequency _start){ user_start = _start.Val(); }
    void setUserStop(Frequency _stop){ user_stop = _stop.Val(); }
    void setCenter(Frequency _center){ center = _center.Val(); updated(this);}
    void setSpan(Frequency _span){ span = _span.Val(); }
	void setFullSpan(){  }
	void setMinSpan(){  }
    void setRBWGrade(int _rbw);
    void setAttenRF(double _atten){ atten = (unsigned int)_atten;  updated(this);}
    void setAttenModeRF(int _mode) {atten_mode = (RFAttenMode)_mode; updated(this);}
    void setAttenCAL(double _atten){ atten_cal = (unsigned int)_atten;  updated(this);}
	void setSweep(bool _sweeping){ sweeping = _sweeping; }	
    int  NptsRequired();
    void startCalibrating() {calibrating = true; updated(this);}
    void stopCalibrating() {calibrating = false; updated(this);}
    void setDFEnabled(bool enabled) {DFEnabled = enabled; updated(this);}
    void setFreqObervIndex(int ind);

signals:
    // Emit when settings successfully changed
    void updated(const DFSettings *);
};


//bool Reconfigure(const DFSettings *s, SCalibrateData & caldata);

#endif
