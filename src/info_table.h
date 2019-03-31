#ifndef INFO_TABLE_H
#define INFO_TABLE_H

#include <atomic>
#include <QWidget>
#include <QTableView>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTimerEvent>
#include <QLineEdit>
#include <QMutex>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include "globals.h"
#include "factory.h"
#include "session.h"

class InfoTable : public QWidget
{
    Q_OBJECT
public:
    explicit InfoTable(Session *sPtr, QWidget *parent = 0);
    void setSamplesEnabled(bool en);

protected:
    virtual void timerEvent( QTimerEvent *e );

private:
    void init();
    Session *session_ptr;

    QCheckBox *use_cal;
    QComboBox *samples_azim;
    QLineEdit *freq_index;
    QStandardItemModel *table_model;
    QTableView *tableView;

    int d_timerId;
    QMutex mutex;
    fvec amplitudes;
    fvec phases;
    int len;
    int offset;
    int atten_rf;
    std::atomic<bool> calibrated;
    std::atomic<bool> samples_loaded;

    Hz current_freq;
    fvec thetas;
    fcube phases_sampl;
    fcube phases_sampl_theor;
    fcube amplitudes_sampl;

signals:

public slots:
    void slotTableUpdated(DataFactory *factory);

private slots:
    void onFreqIndexEditingFinished();

};

#endif // INFO_TABLE_H
