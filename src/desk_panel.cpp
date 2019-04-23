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
    center = new FrequencyEntry(tr("Center"), 100000000);

    connect(center, SIGNAL(freqViewChanged(Frequency)),
        this, SIGNAL(freqChanged(Frequency)));

    observation = new FrequencyEntry(tr("Observation"), center->GetFrequency());

    connect(observation, SIGNAL(freqViewChanged(Frequency)),
        this, SIGNAL(observChanged(Frequency)));

    combo_bw = new ComboEntry(tr("Bandwidth"));
    QStringList bw_str;
    bw_str << "20MHz" << "10MHz" << "5MHz";
    combo_bw->setComboText(bw_str);
    combo_bw->setComboIndex(0);

    combo_rbw = new ComboEntry(tr("RBW"));
    QStringList rbw_str;
    rbw_str << "25kHz" << "12.5kHz" << "6.25kHz" << "3.125kHz";
    combo_rbw->setComboText(rbw_str);
    combo_rbw->setComboIndex(0);

    rx_gain_profile = new ComboEntry(tr("Gain Profile"));
    QStringList profile_str;
    profile_str << "Normal" << "Low Noise" << "Large SFDR";
    rx_gain_profile->setComboText(profile_str);
    rx_gain_profile->setComboIndex(0);

    connect(rx_gain_profile, SIGNAL(comboIndexChanged(int)),
        this, SIGNAL(rmodeChanged(int)));

    rx_agc = new ComboEntry(tr("AGC"));
    QStringList agc_str;
    agc_str << "Manual" << "Auto";
    rx_agc->setComboText(agc_str);
    rx_agc->setComboIndex(1);

    gain = new NumericEntry(tr("Gain"), 40, tr("dB"));
    gain->setEnabled(false);

    fft_avg_cnt = new ComboEntry(tr("FFT Avg."));
    QStringList avg_str;
    avg_str << "8" << "16" << "32";
    fft_avg_cnt->setComboText(avg_str);
    fft_avg_cnt->setComboIndex(0);

    receiver_page->AddWidget(center);
    receiver_page->AddWidget(combo_bw);
    receiver_page->AddWidget(combo_rbw);
    receiver_page->AddWidget(rx_agc);
    receiver_page->AddWidget(gain);
    receiver_page->AddWidget(rx_gain_profile);
//    receiver_page->AddWidget(fft_avg_cnt);
    receiver_page->AddWidget(observation);

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
    calibrator_page->setEnabled(false);

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
    path_page->setEnabled(false);

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
    connect(observation, SIGNAL(freqViewChanged(Frequency)),
            settings, SLOT(setObservation(Frequency)));
    connect(combo_rbw, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setRBWGrade(int)));
    connect(combo_bw, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setBandwidth(int)));
    connect(rx_gain_profile, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setRecvMode(int)));
    connect(rx_agc, SIGNAL(comboIndexChanged(int)),
            settings, SLOT(setAttenModeRF(int)));
    connect(gain, SIGNAL(valueChanged(double)),
            settings, SLOT(setGain(double)));
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
    observation->SetFrequency(settings->Observation());
    combo_rbw->setComboIndex((int)settings->RBWIndex());
    combo_bw->setComboIndex((int)settings->BWIndex());
    rx_gain_profile->setComboIndex((int)settings->GainProfile());
    rx_agc->setComboIndex((int)settings->AttenModeRF());
    gain->SetValue(settings->Gain());
    gain->setEnabled(!settings->AttenModeRF());
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


