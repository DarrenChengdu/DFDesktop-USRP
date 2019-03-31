#ifndef SIGNALGEN_ENTRY_H
#define SIGNALGEN_ENTRY_H

#include "dock_page.h"
#include "entry_widgets.h"
#include "signal_generator.h"

class QEventLoop;

class SignalGenPage : public DockPage
{
    Q_OBJECT
public:
    explicit SignalGenPage(const QString &name,
                           CSignalGenerator *model,
                           QWidget *parent = 0);
    ~SignalGenPage() {}

private:
    void connectInThread(QEventLoop *el);

    IPAddrEntry *ip_addr;
    DualButtonEntry *conn_discon;
    FrequencyEntry *frequency;
    NumericEntry *amplitude;
    CheckBoxEntry *RFOn;
    CheckBoxEntry *sync;

    CSignalGenerator *generator;

signals:

public slots:
    void setGenFrequency(Frequency f);

private slots:
    void onConnectionClicked();
    void onDisconnectionClicked();
    void updatePage(const CSignalGenerator *);
};

#endif // SIGNALGEN_ENTRY_H
