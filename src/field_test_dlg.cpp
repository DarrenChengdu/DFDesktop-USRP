#include "field_test_dlg.h"
#include <QFile>
#include <QtWidgets>
#include <QValueAxis>
#include <iostream>
#include "df_settings.h"
#include "signal_generator.h"
#include "widgets/signalgen_page.h"
#include "newfiledialog.h"
#include "samples_importing_dialog.h"
#include "browse_dlg.h"
#include "simpletable.h"
#include "widgets/progress_dialog.h"

FieldTestDlg::FieldTestDlg(Session *sPtr, QWidget *parent) : session_ptr(sPtr), QMainWindow(parent)
  , samplesLoaded(false)
{
    createMenuBar();

    DockPanel *panel = new DockPanel(tr("Configuration"), this);
    page = new DockPage(tr("Configuration"));
    page->setEnabled(false);

    DFSettings *settings = session_ptr->settings;

    center = new FrequencyEntry(tr("Center"), 100000000);
    connect(center, &FrequencyEntry::freqViewChanged,
            settings, &DFSettings::setCenter);

    offset = new NumericEntry(tr("Offset"), 0, tr("pts"));
    connect(offset, &NumericEntry::valueChanged,
            this, [=](double v) {
        int index = round(v);
//        settings->setFreqObervIndex(index);
    });

    start = new FrequencyEntry(tr("Freq. Start"), 100000000);
    start->setEnabled(false);

    connect(start, &FrequencyEntry::freqViewChanged,
            this, [=](Frequency f)
    {
        if (!single->IsChecked()) {
            if (stop->GetFrequency() < f) {
                stop->SetFrequency(f);
            }

            freqs_v = regspace<Hzvec>(f, step->GetFrequency(), stop->GetFrequency());

            start->SetFrequency(min(freqs_v));
            stop->SetFrequency(max(freqs_v));

            InitFrequencyList(start->GetFrequency(), stop->GetFrequency());
        }        
    });

    stop = new FrequencyEntry(tr("Freq. Stop"), 500000000);
    stop->setEnabled(false);

    connect(stop, &FrequencyEntry::freqViewChanged,
            this, [=](Frequency f)
    {
        if (!single->IsChecked()) {
            if (start->GetFrequency() > f) {
                start->SetFrequency(f);
            }

            freqs_v = regspace<Hzvec>(start->GetFrequency(), step->GetFrequency(), f);
            start->SetFrequency(min(freqs_v));
            stop->SetFrequency(max(freqs_v));
            InitFrequencyList(start->GetFrequency(), stop->GetFrequency());
        }
    });

    step = new FrequencyEntry(tr("Freq. Step"), 1000000);
    step->setEnabled(false);

    connect(step, &FrequencyEntry::freqViewChanged,
            this, [=](Frequency f)
    {
        if (!single->IsChecked()) {
            freqs_v = regspace<Hzvec>(start->GetFrequency(), f, stop->GetFrequency());
            start->SetFrequency(min(freqs_v));
            stop->SetFrequency(max(freqs_v));
            InitFrequencyList(start->GetFrequency(), stop->GetFrequency());
        }
    });

    single = new CheckBoxEntry(tr("Single Freq."));
    single->SetChecked(true);

    connect(single, &CheckBoxEntry::clicked,
            this, [=](bool checked) {
        center->setEnabled(checked);
        offset->setEnabled(checked);
        start->setEnabled(!checked);
        step->setEnabled(!checked);
        stop->setEnabled(!checked);
    });

    thetaExact = new NumericEntry(tr("Current Azim."), 0.0, tr("deg."));

    dual_btn = new DualButtonEntry(tr("Run"), tr("Cancel"));
    dual_btn->LeftButton()->setFocusPolicy(Qt::NoFocus);
    dual_btn->RightButton()->setFocusPolicy(Qt::NoFocus);

    connect(dual_btn, &DualButtonEntry::leftPressed, this, [=]()
    {
        if (single->IsChecked()) {
            setDFEnabled(true);
        }
        else {
            StartStreaming();
        }
    });

    connect(dual_btn, &DualButtonEntry::rightPressed, this, [=]()
    {
        if (single->IsChecked()) {
            setDFEnabled(false);
        }
        else {
            StopStreaming();
        }
    });

    page->AddWidget(center);
    page->AddWidget(offset);
    page->AddWidget(single);
    page->AddWidget(start);
    page->AddWidget(step);
    page->AddWidget(stop);    
    page->AddWidget(thetaExact);
    page->AddWidget(dual_btn);

    panel->AppendPage(page);
    panel->AppendPage(new SignalGenPage(QString(tr("RF Signal Generator")),
                                        new CSignalGenerator));

    textEdit = new QPlainTextEdit;

    connect(this, &FieldTestDlg::updatePlainTextEdit,
            textEdit, &QPlainTextEdit::appendPlainText);
    connect(this, &FieldTestDlg::setDFEnabled,
            settings, &DFSettings::setDFEnabled);

    addDockWidget(Qt::LeftDockWidgetArea, panel);

    corrSeries = new QLineSeries;

    corrChart = new QChart;
    corrChart->legend()->hide();
    corrChart->addSeries(corrSeries);
    corrChart->createDefaultAxes();
    corrChart->setTitle("Curve of CI Algorithm");

    QChartView *chartView = new QChartView(corrChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QStandardItemModel *simpleModel = new QStandardItemModel;
    SimpleTable *sTable = new SimpleTable(simpleModel);
    sTable->setColumnWidth(0, 25);

    connect(session_ptr->settings, &DFSettings::updated,
            this, &FieldTestDlg::updateDlg);

    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    w->setLayout(layout);
    layout->addWidget(chartView);
    layout->addWidget(sTable);
    setCentralWidget(w);

    setWindowTitle(tr("Field Test"));
    updateDlg(settings);
}

void FieldTestDlg::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &FieldTestDlg::newFile);
    fileMenu->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &FieldTestDlg::importFile);
    fileMenu->addAction(openAct);
}

void FieldTestDlg::newFile()
{
    NewFileDialog dlg;
    dlg.exec();
}

void FieldTestDlg::importFile()
{
    FileBrowseDlg dlg;

    if (dlg.exec() == QDialog::Accepted)
    {
        QString path = dlg.getFilePath();

        if (!path.isEmpty()) {
            QString openLabel;
            openLabel = "Loading File\nEstimated 10 seconds\n";

            SHProgressDialog pd(openLabel, this);
            pd.show();

            QEventLoop el;
            std::thread t = std::thread(&FieldTestDlg::samplesFileLocationReceived,
                                        this,
                                        &el,
                                        path);
            el.exec();
            if (t.joinable()) {
                t.join();
            }

            pd.hide();
            page->setEnabled(true);            
        }
    }
}

void FieldTestDlg::updateDlg(const DFSettings *s)
{
    if (single->IsChecked()) {
        center->SetFrequency(s->Center());
        offset->SetValue(s->ObservIndex());
    }

    dual_btn->RightButton()->setEnabled(s->isDFEnabled());
    dual_btn->LeftButton()->setEnabled(!s->isDFEnabled());
}

void FieldTestDlg::onTableChanged(Hz f, int ind_t)
{
    if (samplesLoaded) {

        int ind_f = session_ptr->algorithm->FreqIndex(f);
//        qDebug() << ind_f;

        const float *phases_sampl = session_ptr->algorithm->PhaseSamples();
        const float *amplitudes_sampl = session_ptr->algorithm->AmpSamples();

        QVector<float> PA;
        int Na = session_ptr->algorithm->AntennaCount();
        int Nf = session_ptr->algorithm->FreqCount();
        int Nt = session_ptr->algorithm->ThetaCount();

        for (int n = 0; n < Na*2; n++)
        {
            float val;

            if (n < Na) {
                int ind = ind_f*Na*Nt+n*Nt+ind_t;
                val = phases_sampl[ind];
            }
            else {
                int ind = ind_f*Na*Nt+(n-Na)*Nt+ind_t;
                val = amplitudes_sampl[ind];
            }

            PA.append(val);
        }

        session_ptr->tableSource->setSamplingData(PA);
        const float *thetas = session_ptr->algorithm->Thetas();
        corrChart->axisX()->setRange(thetas[0], thetas[Nt-1]);
        corrChart->axisY()->setRange(-1.1, 1.1);
        corrChart->axisY()->setTitleText(tr("Value of correlation"));
        corrChart->axisX()->setTitleText(tr("Azimuth (deg.)"));
//        QValueAxis *axisX = static_cast<QValueAxis *>(corrChart->axisX());
//        axisX->setTickCount(Nt/4+1);
    }
}

void FieldTestDlg::samplesFileLocationReceived(QEventLoop *el, QString path)
{
    if (session_ptr->settings->isDFEnabled()) {
        session_ptr->settings->setDFEnabled(false);
    }

    session_ptr->tableSource->setSamplingDataEnabled(false);

    if (samplesLoaded)
        samplesLoaded = false;

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

//        samples.save("samples.txt", arma_ascii);
        session_ptr->algorithm->ConfigCircularArray(header.nAnts, header.nLayers, header.radius,
                                                    header.freq_start, header.freq_step, header.freq_stop,
                                                    header.theta_start, header.theta_step, header.theta_stop);

        int nAnts = header.nAnts;
        fcube PhaTabl(Nt, nAnts, Nf);
        fcube AmpTabl(Nt, nAnts, Nf);

        for (int m = 0; m < samples.n_rows; m++)
        {
            for (int n = 0; n < samples.n_cols; n++)
            {
                for (int k = 0; k < samples.n_slices; k++)
                {
                    if (m < nAnts)
                        AmpTabl(k,m,n) = samples(m,n,k);
                    else
                        PhaTabl(k,m-nAnts,n) = samples(m,n,k);
                }
            }
        }

//        PhaTabl.save("PhaTabl.txt", raw_ascii);
        session_ptr->algorithm->ConfigLookupTable(-1.0, PhaTabl.memptr(), AmpTabl.memptr());
    }

    file_handle.close();
    samplesLoaded = true;

    QStringList thetas_str;

    for (int n = 0; n < thetas.size(); n++)
    {
        thetas_str << QString("%1").arg(thetas(n));
    }

    emit setSamplingAzim(thetas_str);

    freqs_full = freqs;
    freqs_full.save("freqs_full.txt", raw_ascii);

    if (!single->IsChecked()) {
        start->SetFrequency(min(freqs_full));
        stop->SetFrequency(max(freqs_full));
        freqs_v = freqs_full;
    }
    else {
        freqs_v << start->GetFrequency();
    }

    offsets_f = linspace<uvec>(0,freqs_full.size()-1,freqs_full.size());

    InitFrequencyList(start->GetFrequency(), stop->GetFrequency());

    session_ptr->tableSource->setSamplingDataEnabled(true);

    while (!el->isRunning()) {
        MSleep(1);
    }

    el->exit();
}

void FieldTestDlg::InitFrequencyList(Frequency _start, Frequency _stop)
{
    DFSettings settings_loc = *session_ptr->settings;

    if (single->IsChecked()) {
        settings_loc.setCenter(_start);
        settings_loc.setWorkMode(WorkMode_FFM);
    }
    else {
        settings_loc.setUserStart(_start);
        settings_loc.setUserStop(_stop);
        settings_loc.setWorkMode(WorkMode_PSCAN);
    }

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

void FieldTestDlg::StartStreaming()
{
    running = true;
    thread_handle = std::thread(&FieldTestDlg::testThread, this);
}

void FieldTestDlg::StopStreaming()
{
    if(thread_handle.joinable()) {
        running = false;
        thread_handle.join();
    }
}

void FieldTestDlg::testThread()
{

}

void FieldTestDlg::onDOAResultsReceived(float bearing, QVector<QPointF> curve)
{
    setWindowTitle(QString(tr("Bearing: %1")).arg(bearing));
    corrSeries->replace(curve);
}
