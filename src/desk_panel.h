#ifndef DESK_PANEL_H
#define DESK_PANEL_H

#include <QEventLoop>
#include "widgets/dock_panel.h"
#include "widgets/entry_widgets.h"
#include "session.h"

#include "armadillo/armadillo"
using namespace arma;


class DeskPanel : public DockPanel {
    Q_OBJECT

public:
    DeskPanel(const QString &title, Session *sPtr,
               QWidget *parent = 0);
    ~DeskPanel();

    int RecvMode() const {return recv_mode->currentIndex();}
    int RFAttenMode() const {return attenModeRF->currentIndex();}
    int RFAtten() const {return attenRF->GetValue();}
    int CalAtten() const {return calibrator_atten->GetValue();}
    int RBW() const {return combo_rbw->currentIndex();}

private:
    void connectInThread(QEventLoop *el);
    void Preset();

	FrequencyEntry *center;
    ComboEntry *combo_rbw;
    ComboEntry *recv_mode;
    ComboEntry *attenModeRF;
    NumericEntry *attenRF;
    ComboEntry *fft_avg_cnt;

    ComboEntry *recv_loop;
    DualButtonEntry *cal_start_stop;
    CheckBoxEntry *calibrator_en;
    CheckBoxEntry *auto_cal;
    NumericEntry *calibrator_atten;

    CheckBoxEntry *ant_switch_auto;
    ComboEntry *ant_layer_sel;

    // Copy of session
    Session *session_ptr;

public slots:


private slots:
    void updatePanel(const DFSettings *);

signals :
    void tunerIDChanged(int);

    void freqChanged(Frequency);
    void startChanged(Frequency);
    void stopChanged(Frequency);
    void stepChanged(Frequency);

    void ifbwChanged(int);
    void wmodeChanged(int);
    void rmodeChanged(int);
    void attenChanged(int, double);

    void ituBWChanged(int);
    void demodChanged(int, int, double);
    void psdProcChanged(int, int, double);
    void psdMeasChanged(int, int, double);
    void decimChanged(double);
    void dgcChecked(bool);
    void squelchChecked(bool);
    void loudspeakerChecked(bool);
    void measModeChanged(int);
    void dgcStatusChanged(bool _en, bool _auto, int grade, double val_dB);
    void squelchChanged(bool _en, bool _auto, double threshold);
    void windowsTypeChanged(int);

    void askforSearching();
    void askforPause();

    void connect2server(const QString &hostName, quint16 port);
    void disconnectFromServer();

private slots:


private:
    DISALLOW_COPY_AND_ASSIGN(DeskPanel)
};

#endif // SWEEP_PANEL_H
