#include "data_collecting_dlg.h"
#include <QFile>
#include <QtWidgets>
#include <iostream>
#include "df_settings.h"
#include "signal_generator.h"
#include "widgets/signalgen_page.h"
#include "newfiledialog.h"
#include "samples_importing_dialog.h"
#include "browse_dlg.h"

CollectingDlg::CollectingDlg(Session *sPtr, QWidget *parent) : session_ptr(sPtr), QMainWindow(parent)
{
    createMenuBar();

    DockPanel *panel = new DockPanel(tr("Configuration"), this);
    page = new DockPage(tr("Configuration"));
    page->setEnabled(false);

    start = new FrequencyEntry(tr("Freq. Start"), 100000000);

    connect(start, &FrequencyEntry::freqViewChanged,
            this, [=](Frequency f)
    {
        uvec ind_1 = arma::find(freqs_full >= (Hz)f);
        uvec ind_2 = arma::find(freqs_full(ind_1) <= (Hz)stop->GetFrequency());
        offsets_f = ind_1(ind_2);
        freqs_v = freqs_full(offsets_f);

        start->SetFrequency(min(freqs_v));
        stop->SetFrequency(max(freqs_v));

        InitFrequencyList(start->GetFrequency(), stop->GetFrequency());
    });

    stop = new FrequencyEntry(tr("Freq. Stop"), 500000000);

    connect(stop, &FrequencyEntry::freqViewChanged,
            this, [=](Frequency f)
    {
        uvec ind_1 = arma::find(freqs_full >= (Hz)start->GetFrequency());
        uvec ind_2 = arma::find(freqs_full(ind_1) <= (Hz)f);
        offsets_f = ind_1(ind_2);
        freqs_v = freqs_full(offsets_f);

        start->SetFrequency(min(freqs_v));
        stop->SetFrequency(max(freqs_v));

        InitFrequencyList(start->GetFrequency(), stop->GetFrequency());
    });

    single = new CheckBoxEntry(tr("Single Freq."));

    connect(single, &CheckBoxEntry::clicked,
            this, [=](bool checked) {
        if (checked) {
            stop->SetFrequency(start->GetFrequency());
        }
        else {
            start->SetFrequency(min(freqs_v));
            stop->SetFrequency(max(freqs_v));
        }
    });

    theta = new ComboEntry(tr("Current Azim."));

    dual_btn = new DualButtonEntry(tr("Run"), tr("Cancel"));
    dual_btn->LeftButton()->setFocusPolicy(Qt::NoFocus);
    dual_btn->RightButton()->setFocusPolicy(Qt::NoFocus);

    connect(dual_btn, &DualButtonEntry::leftPressed, this, [=]()
    {
        dual_btn->LeftButton()->setEnabled(false);
        dual_btn->RightButton()->setEnabled(true);

        StartStreaming();
    });

    connect(dual_btn, &DualButtonEntry::rightPressed, this, [=]()
    {
        StopStreaming();

        dual_btn->RightButton()->setEnabled(false);
        dual_btn->LeftButton()->setEnabled(true);
    });

    page->AddWidget(start);
    page->AddWidget(stop);
    page->AddWidget(single);
    page->AddWidget(theta);
    page->AddWidget(dual_btn);

    panel->AppendPage(page);
    panel->AppendPage(new SignalGenPage(QString(tr("RF Signal Generator")),
                                        new CSignalGenerator));

    textEdit = new QPlainTextEdit;

    connect(this, &CollectingDlg::updatePlainTextEdit,
            textEdit, &QPlainTextEdit::appendPlainText);

    addDockWidget(Qt::LeftDockWidgetArea, panel);

    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    w->setLayout(layout);

    //    layout->addWidget(textEdit);
    setCentralWidget(w);

    //    QVBoxLayout *layout_v = new QVBoxLayout(this);
    //    layout_v->addLayout(layout_h);

    setWindowTitle(tr("Data Collecting"));
}

void CollectingDlg::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &CollectingDlg::newFile);
    fileMenu->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &CollectingDlg::importFile);
    fileMenu->addAction(openAct);
}

void CollectingDlg::newFile()
{
    NewFileDialog dlg;
    dlg.exec();
}

void CollectingDlg::importFile()
{
    FileBrowseDlg dlg;

    if (dlg.exec() == QDialog::Accepted)
    {
        QString path = dlg.getFilePath();

        if (!path.isEmpty()) {
            samplesFileLocationReceived(path);
            page->setEnabled(true);
        }
    }
}

void CollectingDlg::samplesFileLocationReceived(QString path)
{
    samples_location = path;

    QFile file_handle(samples_location);
    file_handle.open(QIODevice::ReadOnly);
    file_handle.seek(20);

    ArrayType type;
    file_handle.read((char *)&type, sizeof(ArrayType));

    Hzvec freqs;
    fvec thetas;

    if (type == ArrayType_Circle)
    {
        circular_array_header header;

        file_handle.seek(0);
        file_handle.read((char *)&header, sizeof(circular_array_header));

        for (int n = 0; n < header.nLayers; n++)
        {
            Hzvec freqs_layer = regspace<Hzvec>(header.freq_start[n],header.freq_step[n],header.freq_stop[n]);
            header.freq_stop[n] = freqs_layer(freqs_layer.size()-1);
            freqs = join_cols(freqs, freqs_layer);
        }

        int Nf = freqs.size();

        thetas = regspace<fvec>(header.theta_start,header.theta_step,header.theta_stop);
        int Nt = thetas.size();
        header.theta_stop = thetas(Nt-1);

        fcube samples(header.nAnts*2, Nf, Nt);

        float current_theta;
        Hz current_freq;

        for (int m = 0; m < Nt; m++)
        {
            for (int n = 0; n < Nf; n++)
            {
                file_handle.read((char *)&current_theta, sizeof(float));
                file_handle.read((char *)&current_freq, sizeof(Hz));

                for (int k = 0; k < header.nAnts*2; k++)
                {
                    float val = 0.0;
                    file_handle.read((char *)&val, sizeof(float));
                    samples(k,n,m) = val;
                }
            }
        }
    }

    file_handle.close();

    QStringList thetas_str;

    for (int n = 0; n < thetas.size(); n++)
    {
        thetas_str << QString("%1").arg(thetas(n));
    }

    theta->setComboText(thetas_str);
    theta->setComboIndex(0);

    freqs_full = freqs;
    freqs_full.save("freqs_full.txt", raw_ascii);

    if (!single->IsChecked()) {
        start->SetFrequency(min(freqs_full));
        stop->SetFrequency(max(freqs_full));
    }

    freqs_v = freqs_full;
    offsets_f = linspace<uvec>(0,freqs_full.size()-1,freqs_full.size());

    InitFrequencyList(start->GetFrequency(), stop->GetFrequency());
}

void CollectingDlg::InitFrequencyList(Frequency _start, Frequency _stop)
{
    DFSettings settings_loc = *session_ptr->settings;
    settings_loc.setUserStart(_start);
    settings_loc.setUserStop(_stop);
    settings_loc.setWorkMode(WorkMode_PSCAN);
    Hz rbw = settings_loc.RBW();
    Hz bw = settings_loc.BW();

    Hzvec centers = settings_loc.Centers();
    centers.save("centers.txt", raw_ascii);

    centers = centers - rbw; // substract one rbw

    centers_v.set_size(freqs_v.size());
    offsets_v.set_size(freqs_v.size());

    // determine center and offset for every frequency in 'freqs_v'
    for (int n = 0; n < freqs_v.size(); n++)
    {
        Hz freq = freqs_v(n);
        vec dist_v = abs(conv_to<vec>::from(centers) - freq);
        Hz dist_min = min(dist_v);
        uvec ind = arma::find(dist_v == dist_min);

        if (ind.size() != 1)
            std::cout << " Warning: ind.size() != 1" << std::endl;
        else {
            centers_v(n) = centers(ind(0)) + rbw;
            offsets_v(n) = (freq - (centers_v(n) - bw/2)) / rbw;
        }
    }

    centers_v.save("centers_v.txt", raw_ascii);
    offsets_v.save("offsets_v.txt", raw_ascii);
}

void CollectingDlg::StartStreaming()
{
    running = true;

    thread_handle = std::thread(&CollectingDlg::CollectingThread, this);
}

void CollectingDlg::StopStreaming()
{
    if(thread_handle.joinable()) {
        running = false;
        thread_handle.join();
        //        cout << "SweepCentral::StopStreaming" << endl;
    }
}

void CollectingDlg::CollectingThread()
{
    // RF Signal Generator
    CSignalGenerator *gen;

    // File Read/Write
    QFile file_handle(samples_location);
    file_handle.open(QIODevice::ReadOnly);
    file_handle.seek(0);

    QFile file_out(samples_location+".tmp");
    file_out.open(QIODevice::WriteOnly);

    file_handle.read((char *)&nants, sizeof(int));
    file_handle.read((char *)&nfreqs, sizeof(int));
    file_handle.read((char *)&nthetas, sizeof(int));
    file_out.write((char *)&nants, sizeof(int));
    file_out.write((char *)&nfreqs, sizeof(int));
    file_out.write((char *)&nthetas, sizeof(int));

    int offset_head = 3;
    int offset_t = theta->currentIndex();

    int offset_rows = offset_t*(2+2*nants)*nfreqs + offsets_f(0);
    int total_rows = nthetas * nfreqs;

    for (int n = 0; n < offset_rows; n++)
    {
        float theta_tmp, value_tmp;
        Hz freq_tmp;

        file_handle.read((char *)&theta_tmp, sizeof(float));
        file_handle.read((char *)&freq_tmp, sizeof(Hz));
        file_out.write((char *)&theta_tmp, sizeof(float));
        file_out.write((char *)&freq_tmp, sizeof(Hz));

        for (int m = 0; m < nants*2; m++)
        {
            file_handle.read((char *)&value_tmp, sizeof(float));
            file_out.write((char *)&value_tmp, sizeof(float));
        }
    }

    int timeout_cnt = 100;
    int fifo_depth = 5;
    float threshold_p = 3.0;

    // 1. load samples data from file named 'path'
    Hz center_last = centers_v(0);
    session_ptr->settings->setWorkMode(WorkMode_FFM);
    session_ptr->settings->setCenter(center_last);

    MSleep(100); // wait for reconfiguration completed in sweep_central

    mat phases_fifo(fifo_depth, NUM_ANTENNAS, fill::zeros);
    mat amplitudes_fifo(fifo_depth, NUM_ANTENNAS, fill::zeros);

    int current_band = 0;
    bool success = true;

    for (int n = 0; n < offsets_f.size(); n++)
    {
        if (!running) {
            success = false;
            break;
        }

        int offset_g = offsets_f(n);

        // 1. Config RF Signal Generator
        float f_mhz = freqs_full(offset_g) * 1.0e-6;
        gen->SetSignalGenFreq(f_mhz);
        MSleep(10);

        // 2. Receiver Ctrl
        if (centers_v(n) != center_last && n > 0)
        {
            session_ptr->settings->setCenter(centers_v(n));

            center_last = centers_v(n);
            current_band++;
        }
        else
        {
            raw_mtx.lock();
            data_frames_raw.reset();
            raw_mtx.unlock();

            //            snapshot_mtx.lock();
            //            amplitudes_q.reset();
            //            phases_q.reset();
            //            snapshot_mtx.unlock();
        }

        int fifo_cnt = 0;
        int offset_l = offsets_v(n);

        // 2.

        while (fifo_cnt < timeout_cnt)
        {
            snapshot_mtx.lock();
            if (phases_q.read_available() > fifo_depth && amplitudes_q.read_available() > fifo_depth)
            {
                for (int i = 0; i < fifo_depth; i++)
                {
                    mat phases_tmp;
                    phases_q.pop(phases_tmp);

                    mat amplitudes_tmp;
                    amplitudes_q.pop(amplitudes_tmp);

                    for (int j = 0; j < NUM_ANTENNAS; j++)
                    {
                        phases_fifo(i, j) = phases_tmp(offset_l, j);
                        amplitudes_fifo(i, j) = amplitudes_tmp(offset_l, j);
                    }
                }
                snapshot_mtx.unlock();

                fifo_cnt++;
                double stddev_max = (stddev(phases_fifo, 0, 0)).max();

                cout << "stddev_max = " << stddev_max << endl;

                if (stddev_max < threshold_p)
                {
                    rowvec amp_mean = arma::mean(amplitudes_fifo, 0);
                    rowvec pha_mean = arma::mean(phases_fifo, 0);

                    file_out.write((char *)&thetas_v(offset_t), sizeof(float));
                    file_out.write((char *)&freqs_full(offset_g), sizeof(Hz));

                    for (int m = 0; m < nants*2; m++)
                    {
                        float value_tmp = 0.0;

                        if (m < nants)
                            value_tmp = amp_mean(m);
                        else
                            value_tmp = pha_mean(m-nants);

                        file_out.write((char *)&value_tmp, sizeof(float));
                        samples(m,offset_g,offset_t) = value_tmp;
                    }

                    break;
                }
            }
            else
            {
                snapshot_mtx.unlock();
                MSleep(1);
            }
        }

        cout << "fifo_cnt = " << fifo_cnt << endl;
    }

    if (success)
    {
        file_handle.seek((offset_head+offsets_f.size()*(2+2*nants))*4);

        for (int n = offset_rows+offsets_f.size(); n < total_rows; n++)
        {
            float theta_tmp, value_tmp;
            Hz freq_tmp;

            file_handle.read((char *)&theta_tmp, sizeof(float));
            file_handle.read((char *)&freq_tmp, sizeof(Hz));
            file_out.write((char *)&theta_tmp, sizeof(float));
            file_out.write((char *)&freq_tmp, sizeof(Hz));

            for (int m = 0; m < nants*2; m++)
            {
                file_handle.read((char *)&value_tmp, sizeof(float));
                file_out.write((char *)&value_tmp, sizeof(float));
            }
        }
    }

    file_handle.close();
    file_out.close();

    if (success) {
        emit dataCollected(samples, thetas, freqs);
        emit updatePlainTextEdit(tr("Data collections succeeded!"));
    }
    else {
        emit updatePlainTextEdit(tr("Data collections failed!"));
    }
}
