#include "sweep_central.h"
#include "df_lib.h"

SweepCentral::SweepCentral(Session *sPtr, QObject *parent) : QObject(parent),
    session_ptr(sPtr), running(false), north_cal(0.0), reconfigure(false)
{
    data_factory = new DataFactory(session_ptr);
}

void SweepCentral::settingsChanged(const DFSettings *s)
{
    reconfigure = true;
}

void SweepCentral::StartStreaming()
{
    running = true;
    thread_handle = std::thread(&SweepCentral::SweepThread, this);
}

void SweepCentral::StopStreaming()
{
    if(thread_handle.joinable()) {
        running = false;
        thread_handle.join();
    }
}

void SweepCentral::SweepThread()
{
    IntermediatePacket frame_raw;
    Reconfigure();

    while (running)
    {
        if(reconfigure)
        {
            Reconfigure();
        }

        raw_mtx.lock();
        if (preprocessed_data.read_available() > 0)
        {
            preprocessed_data.pop(frame_raw);
            raw_mtx.unlock();

            // push data frames into data cells
            data_factory->Push(frame_raw);

            if (data_factory->AllDone())
            {
                std::cout << "All done!" << std::endl;
                if (data_factory->IsCalibrating())
                {
                    // plan to stop if it was calibrating before
                    if (session_ptr->settings->isCalibratingAuto())
                    {
                        data_factory->SwitchToCAL(false);
//                        session_ptr->device->StopCalibrating();

                        MSleep(100); // 等待设备完全响应之后再彻底清空缓存 preprocessed_data
                    }

                    preprocessed_data.reset();
                }
                else
                {
                    fmat ampCorrected = data_factory->amplitudes;// - data_factory->amplitudes_cal;
                    mat amplitudes = conv_to<mat>::from(ampCorrected);

                    amplitudes_q.push(amplitudes);

                    fmat pha_corrected = data_factory->phases - data_factory->phases_cal;

                    for (int m = 0; m < pha_corrected.n_rows; m++)
                    {
                        for (int n = 0; n < pha_corrected.n_cols; n++)
                        {
                            pha_corrected(m,n) = df_lib::angle(pha_corrected(m,n));
                        }
                    }

                    mat phases = conv_to<mat>::from(pha_corrected);

                    phases_q.push(phases);

                    int nchannels = ampCorrected.n_cols;
                    int npoints = ampCorrected.n_rows;

                    frowvec tableAmp = data_factory->amplitudes.row(observIndex);
                    frowvec tableAmpCal = data_factory->amplitudes_cal.row(observIndex);
                    frowvec tablePha = data_factory->phases.row(observIndex);
                    frowvec tablePhaCal = data_factory->phases_cal.row(observIndex);

                                 QVector<QVector<QPointF> > seriesData;

                    for (int k = 0; k < nchannels; k++)
                    {
                        QVector<QPointF> points;

                        for (int i = 0; i < npoints; i++) {
                            points.append(QPointF(freqs_v(i), ampCorrected(i,k)));
                        }

                        seriesData.append(points);
                    }

                    session_ptr->dataSource->pushData(seriesData);
                    session_ptr->tableSource->pushData(tableAmp.memptr(),
                                                       tableAmpCal.memptr(),
                                                       tablePha.memptr(),
                                                       tablePhaCal.memptr());

                    if (data_factory->isDFEnabled()) {
                        int offset = session_ptr->settings->FreqObservIndex();
                        float bearing = data_factory->bearings(offset);

                        fvec curve = data_factory->corrCurve.col(offset);
                        const float *theatas = session_ptr->algorithm->Thetas();

                        QVector<QPointF> curve_v;

                        for (int i = 0; i < curve.size(); i++) {
                            curve_v.append(QPointF(theatas[i], curve(i)));
                        }

                        emit doaResults(bearing, curve_v);
                    }
                }
            }
        }
        else
        {
            raw_mtx.unlock();
            MSleep(10);
        }
    }
}

void SweepCentral::Reconfigure()
{
    if (!session_ptr->device)
        return;

    DFSettings *s = session_ptr->settings;

    if(session_ptr->device->Reconfigure(s))
    {        
        data_factory->Init(NUM_ANTENNAS, native_dsp_lut[s->RBWIndex()].fft_size, s->RBW(), s->Centers());
        data_factory->setDFEnabled(s->isDFEnabled());

        Hzvec f_list = s->FreqList();
        observIndex = s->FreqObservIndex();
        session_ptr->dataSource->setMarkerPos(observIndex);
        session_ptr->tableSource->setFreqObserv(s->FreqObserv());

        freqs_v = conv_to<vec>::from(f_list) * 1.0e-6;
        emit plotsAxisXUpdated(freqs_v.min(), freqs_v.max());

        session_ptr->dataSource->clear();

//        Sleep(10);

        if (s->isCalibratingAuto())
        {
            // device switch to calibration
//            session_ptr->device->StartCalibrating(s->TypeCAL());

            // data factory switch to calibration
//            data_factory->SwitchToCAL(true);
        }
        else
        {
//            data_factory->SwitchToCAL(s->isCalibrating());
        }

        preprocessed_data.reset();

        snapshot_mtx.lock();
        amplitudes_q.reset();
        phases_q.reset();
        snapshot_mtx.unlock();

        // print running informations
        PrintSettings(s);
    }
//    else {
//        // device rolls back to previous state
//        session_ptr->device->Reconfigure(session_ptr->settings);
//    }

    reconfigure = false;
}

void SweepCentral::PrintSettings(const DFSettings *s)
{
    MY_INFO << tr("Details Printing...");
    MY_INFO << tr("Work Mode: ") << s->Mode() << tr("<0-FFM; 1-PScan>");

    if (s->Mode() == WorkMode_FFM) {
        MY_INFO << tr("Frequency Center: ") << s->Center() << "Hz";
    }

    if (s->Mode() == WorkMode_PSCAN) {
        MY_INFO << tr("Frequency Start: ") << s->Start() << "Hz";
        MY_INFO << tr("Frequency Stop: ") << s->Stop() << "Hz";
        MY_INFO << tr("Dwell Time: ") << s->DwellTime() << "ms";
    }

    MY_INFO << tr("Bandwidth: ") << s->BW() << "Hz";
    MY_INFO << tr("FFT Bin: ") << s->RBWIndex() << "<0-25kHz; 1-12.5kHz; 2-6.25kHz; 3-3.125kHz>";
    MY_INFO << tr("RF Atte. Mode: ") << s->AttenModeRF() << tr("<0-Manual; 1-Auto>");
    MY_INFO << tr("RF Atte. Value (dB): ") << s->dBAttenRF();
    MY_INFO << tr("Receiver Mode: ") << s->RecvMode() << tr("<0-Common; 1-Low Noise; 2-Large Dynamic>");
    MY_INFO << tr("Average Count: ") << s->AvgCount() << tr("<0-avg_8; 1-avg_16; 2-avg_32>");
    MY_INFO << tr("Calibrator Enabled: ") << s->CalibratorEnabled();
    MY_INFO << tr("Calibrator Atten.: ") << s->dBAttenCAL();
    MY_INFO << tr("Calibration Type: ") << s->TypeCAL();
}
