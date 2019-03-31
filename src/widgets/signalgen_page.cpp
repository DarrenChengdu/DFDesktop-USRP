#include "signalgen_page.h"
#include "widgets/progress_dialog.h"
#include <QtWidgets>
#include "globals.h"

SignalGenPage::SignalGenPage(const QString &name,
                             CSignalGenerator *model,
                             QWidget *parent) : DockPage(name, parent)
  , generator(model)
{
    ip_addr = new IPAddrEntry(tr("IP addr."), "192.168.10.101");
    conn_discon = new DualButtonEntry(tr("Connect"), tr("Disconnect"));

    frequency = new FrequencyEntry(tr("Frequency"), 100000000);
    amplitude = new NumericEntry(tr("Amplitude"), -50.0, "dBm");
    RFOn = new CheckBoxEntry(tr("RF On"));

    sync = new CheckBoxEntry(tr("Sync. with"));
    sync->SetChecked(true);

    connect(conn_discon, SIGNAL(leftPressed()),
            this, SLOT(onConnectionClicked()));
    connect(conn_discon, SIGNAL(rightPressed()),
            this, SLOT(onDisconnectionClicked()));
    connect(frequency, SIGNAL(freqViewChanged(Frequency)),
            generator, SLOT(SetSignalGenFreq(Frequency)));
    connect(amplitude, SIGNAL(valueChanged(double)),
            generator, SLOT(SetSignalGenPow(double)));
    connect(RFOn, SIGNAL(clicked(bool)),
            generator, SLOT(SetSignalRFOn(bool)));

    connect(generator, SIGNAL(updated(const CSignalGenerator*)),
            this, SLOT(updatePage(const CSignalGenerator*)));

    AddWidget(ip_addr);
    AddWidget(conn_discon);
    AddWidget(frequency);
    AddWidget(amplitude);
    AddWidget(RFOn);
    AddWidget(sync);

//    updatePage(session->generator_1);
}

void SignalGenPage::updatePage(const CSignalGenerator *gen)
{
    bool connected = gen->isConnected();

    conn_discon->LeftButton()->setEnabled(!connected);
    conn_discon->RightButton()->setEnabled(connected);
    ip_addr->setEnabled(!connected);

    frequency->SetFrequency(gen->getFrequency());
    amplitude->SetValue(gen->getAmplitude());
    RFOn->SetChecked(gen->isRFOn());

    frequency->setEnabled(connected);
    amplitude->setEnabled(connected);
    RFOn->setEnabled(connected);
    sync->setEnabled(connected);
}

void SignalGenPage::setGenFrequency(Frequency f) {
    if (sync->IsChecked()) {
        generator->SetSignalGenFreq(f);
    }
}

void SignalGenPage::onConnectionClicked()
{
    if (!generator) {
        QMessageBox::critical(this, tr("SignalGenPage"),
                                 tr("generator is still null!"));
        return;
    }

    if (ip_addr->GetIP().isEmpty()) {
        QMessageBox::warning(this, tr("Connection Status"),
                                 tr("IP address is empty!"));
        return;
    }

    QString openLabel;
    openLabel = "Connecting Device\nEstimated 10 seconds\n";

    SHProgressDialog pd(openLabel, this);
    pd.show();

    QEventLoop el;
    std::thread t = std::thread(&SignalGenPage::connectInThread,
                                this,
                                &el);
    el.exec();
    if (t.joinable()) {
        t.join();
    }

    pd.hide();
}

void SignalGenPage::connectInThread(QEventLoop *el)
{
    QString str = ip_addr->GetIP();
    QString strFull = "TCPIP0::" + str + "::inst0::INSTR";

    generator->ConnectSignalGen(strFull.toLatin1().data());

    while (!el->isRunning()) {
        MSleep(1);
    }

    el->exit();
}

void SignalGenPage::onDisconnectionClicked()
{
    generator->DisconnectSignalGen();
}
