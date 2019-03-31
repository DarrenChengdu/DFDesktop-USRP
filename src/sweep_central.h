#ifndef SWEEP_CENTRAL_H
#define SWEEP_CENTRAL_H

#include <QObject>
#include <QThread>
#include <thread>
#include <mutex>
#include "session.h"
#include "factory.h"

class SweepCentral : public QObject
{
    Q_OBJECT
public:
    explicit SweepCentral(Session *sPtr, QObject *parent = 0);
    bool IsRunning() {return running;}
    void StartStreaming();
    void StopStreaming();

private:
    void SweepThread();
    void Reconfigure();
    void PrintSettings(const DFSettings *s);

    float north_cal;
    std::mutex northMutex;

    std::thread thread_handle;
    // Copy of session
    Session *session_ptr;
    bool running;

    bool reconfigure;
    int observIndex;

    vec freqs_v;
    DataFactory *data_factory;
//    DFSettings last_config;

signals:
    void doaResults(float, QVector<QPointF>);
    void plotsAxisYUpdated(mat);
    void plotsAxisXUpdated(double xmin, double xmax);

private slots:


public slots:
//    void slotCompassUpdated(float);
    void settingsChanged(const DFSettings *s);
};

#endif // SWEEP_CENTRAL_H
