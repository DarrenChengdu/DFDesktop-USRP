#ifndef TABLESOURCE_H
#define TABLESOURCE_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QTimerEvent>
#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <atomic>
#include "globals.h"

class TableSource : public QObject
{
    Q_OBJECT
public:
    explicit TableSource(int channelCount, QObject *parent = nullptr);
    ~TableSource();

    int ChannelCount() const {return nchannels;}
    void pushData(float *_amp,
                  float *_amp_cal,
                  float *_pha,
                  float *_pha_cal);
    void setModel(QStandardItemModel *model);

protected:
    virtual void timerEvent( QTimerEvent *e );

private:
    int nchannels;
    int timerID;
    QStandardItemModel *tableModel;
    
    std::atomic<bool> samplesLoaded;
    std::atomic<bool> corrected;
    std::atomic<int> pointIndex;
    int channelCount;

    float *amplitudes;
    float *phases;
    float *amplitudes_cal;
    float *phases_cal;
    float *phasesStored;
    float *amplitudesStored;

    int azimIndex;
    Hz freqObserv;

    QMutex dataMutex;


signals:
    void tableChanged(Hz, int);

public slots:
    void setFreqObserv(Hz);
    void setSamplingDataEnabled(bool);
    void setSamplingData(QVector<float>);
    void setCALEnabled(bool sta);
    void setAzimuthIndex(int ind);
};

#endif // TABLESOURCE_H
