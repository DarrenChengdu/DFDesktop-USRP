#include "tablesource.h"
#include "df_lib.h"
#include <QStandardItem>
#include <QModelIndex>
#include <QDebug>

TableSource::TableSource(int channelCount, QObject *parent) : QObject(parent)
  , tableModel(0), nchannels(channelCount), corrected(0), samplesLoaded(false)
  , azimIndex(-1), freqObserv(0)
{
    pointIndex = 0;

    amplitudes = new float [nchannels];
    amplitudes_cal = new float [nchannels];
    phases = new float [nchannels];
    phases_cal = new float [nchannels];
    phasesStored = new float [nchannels];
    amplitudesStored = new float [nchannels];

    memset((char *)amplitudes, 0, 4*nchannels);
    memset((char *)amplitudes_cal, 0, 4*nchannels);
    memset((char *)phases, 0, 4*nchannels);
    memset((char *)phases_cal, 0, 4*nchannels);
    memset((char *)phasesStored, 0, 4*nchannels);
    memset((char *)amplitudesStored, 0, 4*nchannels);
}

TableSource::~TableSource()
{
    killTimer(timerID);

    delete []amplitudes;
    delete []amplitudes_cal;
    delete []phases;
    delete []phases_cal;
    delete []phasesStored;
    delete []amplitudesStored;
}

void TableSource::setModel(QStandardItemModel *model)
{
    tableModel = model;

    for (int n = 0; n < nchannels; n++) {
        QStandardItem *phaItem = new QStandardItem(QString::number(0.0, 'f', 1));
        phaItem->setTextAlignment(Qt::AlignCenter);
        tableModel->setItem(n, 1, phaItem);

        QStandardItem *phaCalItem = new QStandardItem(QString::number(0.0, 'f', 1));
        phaCalItem->setTextAlignment(Qt::AlignCenter);
        tableModel->setItem(n, 2, phaCalItem);

        QStandardItem *phaStoredItem = new QStandardItem(QString::number(0.0, 'f', 1));
        phaStoredItem->setTextAlignment(Qt::AlignCenter);
        tableModel->setItem(n, 3, phaStoredItem);

        QStandardItem *ampItem = new QStandardItem(QString::number(0.0, 'f', 1));
        ampItem->setTextAlignment(Qt::AlignCenter);
        tableModel->setItem(n, 5, ampItem);

        QStandardItem *ampCalItem = new QStandardItem(QString::number(0.0, 'f', 1));
        ampCalItem->setTextAlignment(Qt::AlignCenter);
        tableModel->setItem(n, 6, ampCalItem);

        QStandardItem *ampStoredItem = new QStandardItem(QString::number(0.0, 'f', 1));
        ampStoredItem->setTextAlignment(Qt::AlignCenter);
        tableModel->setItem(n, 7, ampStoredItem);
    }

    timerID = startTimer( ceil(1000/5.0) );
}

void TableSource::pushData(float *_amp,
                           float *_amp_cal,
                           float *_pha,
                           float *_pha_cal)
{
    dataMutex.lock();

    memcpy((char *)amplitudes, (char *)_amp, 4*nchannels);
    memcpy((char *)amplitudes_cal, (char *)_amp_cal, 4*nchannels);
    memcpy((char *)phases, (char *)_pha, 4*nchannels);
    memcpy((char *)phases_cal, (char *)_pha_cal, 4*nchannels);

    dataMutex.unlock();
}

void TableSource::setCALEnabled(bool sta)
{
    corrected = sta;
}

void TableSource::setSamplingDataEnabled(bool sta) {
    samplesLoaded = sta;
}

void TableSource::setSamplingData(QVector<float> dataStored)
{    
    dataMutex.lock();
    for (int n = 0; n < nchannels; n++) {
        phasesStored[n] = dataStored.at(n);
        amplitudesStored[n] = dataStored.at(n+nchannels);
    }

    dataMutex.unlock();
}

void TableSource::timerEvent( QTimerEvent *e )
{    
    if (e->timerId() == timerID)
    {
        dataMutex.lock();
        for (int n = 0; n < nchannels; n++)
        {
            float pha = phases[n];

            if (corrected) {
                pha = df_lib::angle(pha - phases_cal[n]);
            }

            QModelIndex ind1 = tableModel->index(n,1);
            tableModel->setData(ind1, QVariant(QString::number(pha, 'f', 1)));

            QModelIndex ind2 = tableModel->index(n,2);
            tableModel->setData(ind2,  QVariant(QString::number(phases_cal[n], 'f', 1)));

            QModelIndex ind3 = tableModel->index(n,3);
            QModelIndex ind7 = tableModel->index(n,7);

            if (samplesLoaded) {
                tableModel->setData(ind3,  QVariant(QString::number(phasesStored[n], 'f', 1)));
                tableModel->setData(ind7, QVariant(QString::number(amplitudesStored[n], 'f', 1)));
            }

            else {
                tableModel->setData(ind3,  QVariant(QString(tr("Null"))));
                tableModel->setData(ind7,  QVariant(QString(tr("Null"))));
            }

            float amp = amplitudes[n];

            if (corrected) {
                amp = amp - amplitudes_cal[n];
            }

            QModelIndex ind5 = tableModel->index(n,5);
            tableModel->setData(ind5, QVariant(QString::number(amp, 'f', 1)));

            QModelIndex ind6 = tableModel->index(n,6);
            tableModel->setData(ind6, QVariant(QString::number(amplitudes_cal[n], 'f', 1)));
        }
        dataMutex.unlock();
    }
}



void TableSource::setAzimuthIndex(int ind)
{    
    if (azimIndex == ind)
        return;

    azimIndex = ind;
    emit tableChanged(freqObserv, azimIndex);
}



void TableSource::setFreqObserv(Hz f)
{    
    if (freqObserv == f)
        return;

    freqObserv = f;
    emit tableChanged(freqObserv, azimIndex);
}

