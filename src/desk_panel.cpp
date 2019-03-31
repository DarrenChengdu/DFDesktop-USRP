#include "desk_panel.h"
#include "widgets/progress_dialog.h"
#include "df_lib.h"
#include "widgets/signalgen_page.h"
#include <QMessageBox>
#include <thread>

DeskPanel::DeskPanel(const QString &title, Session *sPtr,
                     QWidget *parent)
    : DockPanel(title, parent), session_ptr(sPtr)
{
    DockPage *receiver_page = new DockPage(tr("Multi-channel Receiver"));
    DockPage *calibrator_page = new DockPage(tr("Calibrator"));
    DockPage *path_page = new DockPage(tr("Path Selection"));

    // receiver page
    center = new FrequencyEntry(tr("Central Freq"), 100000000);

    connect(center, SIGNAL(freqViewChanged(Frequency)),
        this, SIGNAL(freqChanged(Frequency)));

    combo_rbw = new ComboEntry(tr("RBW"));
    QStringList rbw_str;
    rbw_str << "25kHz" << "12.5kHz" << "6.25kHz" << "3.125kHz";
    combo_rbw->setComboText(rbw_str);
    combo_rbw->setComboIndex(0);

    recv_mode = new ComboEntry(tr("Rcv. Mode"));
    QStringList rmode_str;
    rmode_str << "Normal" << "Low Noise" << "Large SFDR";
    recv_mode->setComboText(rmode_str);
    recv_mode->setComboIndex(0);

    connect(recv_mode, SIGNAL(comboIndexChanged(int)),
        this, SIGNAL(rmodeChanged(int)));

    attenModeRF = new ComboEntry(tr("Atten. Mode"));
    QStringList amode_str;
    amode_str << "Manual" << "Auto";
    attenModeRF->setComboText(amode_str);
    attenModeRF->setComboIndex(1);

    attenRF = new NumericEntry(tr("RF Atten."), 0, tr("dB"));
    attenRF->setEnabled(false);

    fft_avg_cnt = new ComboEntry(tr("FFT Avg."));
    QStringList avg_str;
    avg_str << "8" << "16" << "32";
    fft_avg_cnt->setComboText(avg_str);
    fft_avg_cnt->setComboIndex(0);

    receiver_page->AddWidget(center);
    receiver_page->AddWidget(combo_rbw);
    receiver_page->AddWidget(attenModeRF);
    receiver_page->AddWidget(attenRF);
    receiver_page->AddWidget(recv_mode);
    receiver_page->AddWidget(fft_avg_cnt);

    // calibrator page
    cal_start_stop = new DualButtonEntry(tr("CAL Start"), tr("CAL Stop"));
    calibrator_en = new CheckBoxEntry(tr("Enable/Disable"));
    calibrator_atten = new NumericEntry(tr("Atten."), 10, tr("dB"));
    auto_cal = new CheckBoxEntry(tr("Auto CAL"));

    recv_loop = new ComboEntry(tr("CAL Type"));
    QStringList loop_str;
    loop_str << "External" << "Internal";
    recv_loop->setComboText(loop_str);
    recv_loop->setComboIndex(1);

    calibrator_page->AddWidget(calibrator_en);
    calibrator_page->AddWidget(calibrator_atten);
    calibrator_page->AddWidget(recv_loop);
    calibrator_page->AddWidget(cal_start_stop);
    calibrator_page->AddWidget(auto_cal);

    // path page
    ant_switch_auto = new CheckBoxEntry(tr("Ant. Auto"));
    ant_switch_auto->SetChecked(true);
    ant_layer_sel = new ComboEntry(tr("Ant. Layer"));
    QStringList layer_str;
    layer_str << "First" << "Second" << "Third";
    ant_layer_sel->setComboText(layer_str);
    ant_layer_sel->setComboIndex(0);
    ant_layer_sel->setEnabled(false);

    path_page->AddWidget(ant_switch_auto);
    path_page->AddWidget(ant_layer_sel);

    SignalGenPage *generator_page = new SignalGenPage(QString(tr("RF Signal Generator")),
                                                new CSignalGenerator);

    // RF Signal Generator Page
    AppendPage(receiver_page);
    AppendPage(calibrator_page);
    AppendPage(path_page);
    AppendPage(generator_page);

    const DFSettings *settings = session_ptr->settings;

    // slots
    connect(center, SIGNAL(freqViewChanged(Frequency)),
            settings, SLOT(setCenter(Frequency)));
    connect(center, SIGNAL(freqViewChanged(Frequency)),
            generator_page, SLOT(setGenFrequency(Frequency)));
    connect(combo_rbw, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setRBWGrade(int)));
    connect(recv_mode, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setRecvMode(int)));
    connect(attenModeRF, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setAttenModeRF(int)));
    connect(attenRF, SIGNAL(valueChanged(double)),
            settings, SLOT(setAttenRF(double)));
    connect(fft_avg_cnt, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setFFTAvgCnt(int)));
    connect(cal_start_stop, SIGNAL(leftPressed()),
            settings, SLOT(startCalibrating()));
    connect(cal_start_stop, SIGNAL(rightPressed()),
            settings, SLOT(stopCalibrating()));
    connect(calibrator_en, SIGNAL(clicked(bool)),
            settings, SLOT(setCalibratorEnabled(bool)));
    connect(calibrator_atten, SIGNAL(valueChanged(double)),
            settings, SLOT(setAttenCAL(double)));
    connect(auto_cal, SIGNAL(clicked(bool)),
            settings, SLOT(setCalibratingAuto(bool)));
    connect(recv_loop, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setCalibration(int)));
    connect(ant_switch_auto, SIGNAL(clicked(bool)),
            settings, SLOT(setAntSwitchAuto(bool)));
    connect(ant_layer_sel, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setAntLayer(int)));

    connect(settings, SIGNAL(updated(const DFSettings*)),
            this, SLOT(updatePanel(const DFSettings*)));

    updatePanel(settings);
}

DeskPanel::~DeskPanel()
{
    // Don't delete widgets on pages
}

void DeskPanel::updatePanel(const DFSettings *settings)
{
    center->SetFrequency(settings->Center());
    combo_rbw->setComboIndex((int)settings->RBWIndex());
    recv_mode->setComboIndex((int)settings->RecvMode());
    attenModeRF->setComboIndex((int)settings->AttenModeRF());
    attenRF->SetValue(settings->dBAttenRF());
    attenRF->setEnabled(!settings->AttenModeRF());
    fft_avg_cnt->setComboIndex((int)settings->AvgCount());
    calibrator_en->SetChecked(settings->CalibratorEnabled());
    recv_loop->setComboIndex(settings->TypeCAL());
    auto_cal->SetChecked(settings->isCalibratingAuto());
    ant_switch_auto->SetChecked(settings->AntSwitchAuto());
    ant_layer_sel->setEnabled(!settings->AntSwitchAuto());
    ant_layer_sel->setComboIndex(settings->CurrentAntLayer());

    bool sta = settings->isCalibratingAuto();
    bool sta2 = settings->isCalibrating();

    calibrator_en->setEnabled(!sta);
    recv_loop->setEnabled(!sta);
    calibrator_atten->setEnabled(!sta);
    cal_start_stop->LeftButton()->setEnabled(!sta);
    cal_start_stop->RightButton()->setEnabled(!sta);
    auto_cal->setEnabled(!sta2);

    if (!sta) {
        cal_start_stop->LeftButton()->setEnabled(!sta2);
        cal_start_stop->RightButton()->setEnabled(sta2);
    }
}


