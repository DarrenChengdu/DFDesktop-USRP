#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QFile>
#include "widgets/entry_widgets.h"
#include "widgets/dock_panel.h"
#include "globals.h"
#include "libci.h"
#include <thread>

const int MAX_LAYERS = 3;

// Version 1 header
struct circular_array_header {
    unsigned short signature;
    unsigned short version;
    qint64 time_created;
    qint64 time_modified;
    ARRAY_TYPE type;
    short nAnts;
    float theta_start;
    float theta_step;
    float theta_stop;
    short nLayers;
    float radius[MAX_LAYERS];
    Hz freqs_start[MAX_LAYERS];
    Hz freqs_step[MAX_LAYERS];
    Hz freqs_stop[MAX_LAYERS];
};

class QEventLoop;

class NewFileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewFileDialog(QWidget *parent = 0);
    int exec();

private:
    void setupNewTable(QEventLoop *el);

    IAlgorithmCI *algorithm;
    std::thread thread_handle;

    DockPage *page;
    ComboEntry *array_type;
    CheckBoxEntry *use_theor;
    NumericEntry *ants_count;
    ComboEntry *array_layers;
    NumericEntry *array_dia_0;
    NumericEntry *array_dia_1;
    NumericEntry *array_dia_2;
    FrequencyEntry *start_freq_0;
    FrequencyEntry *step_freq_0;
    FrequencyEntry *stop_freq_0;
    FrequencyEntry *start_freq_1;
    FrequencyEntry *step_freq_1;
    FrequencyEntry *stop_freq_1;
    FrequencyEntry *start_freq_2;
    FrequencyEntry *step_freq_2;
    FrequencyEntry *stop_freq_2;
    NumericEntry *start_azim;
    NumericEntry *step_azim;
    NumericEntry *stop_azim;

    DualButtonEntry *apply_or_close;
    QFile file_handle;

signals:

private slots:
    void togglePage();

public slots:
};

#endif // NEWFILEDIALOG_H
