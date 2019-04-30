#ifndef FIELD_TEST_DLG_H
#define FIELD_TEST_DLG_H

#include <atomic>
#include <thread>
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGraphicsGridLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtCharts/QLineSeries>

#include "session.h"
#include "widgets/dock_panel.h"
#include "widgets/entry_widgets.h"

class QEventLoop;

QT_CHARTS_USE_NAMESPACE

class FieldTestDlg : public QMainWindow
{
    Q_OBJECT
public:
    explicit FieldTestDlg(Session *sPtr, QWidget *parent = 0);

private:
    void createMenuBar();
    void StartStreaming();
    void StopStreaming();
    void testThread();
    void InitFrequencyList(Frequency, Frequency);

//    NewFileDialog *newFileDlg;
    QLineSeries *corrSeries;
    QChart *corrChart;

    DockPage *page;
    Session *session_ptr;
    CheckBoxEntry *single;
    FrequencyEntry *center;
    FrequencyEntry *observation;
    FrequencyEntry *start;
    FrequencyEntry *step;
    FrequencyEntry *stop;
    NumericEntry *thetaExact;
    DualButtonEntry *dual_btn;
    QPlainTextEdit *textEdit;
    QString samples_location;

    int nants, nfreqs, nthetas;
    fvec thetas_v;
    Hzvec freqs_v;
    uvec offsets_f;
    Hzvec freqs_full;
    Hzvec centers_v;
    uvec offsets_v;
    std::thread thread_handle;
    std::atomic<bool> running;

    fcube samples;
    fmat  thetas;
    Hzmat freqs;
    bool samplesLoaded;

signals:
    void setSamplingAzim(QStringList);
    void setDFEnabled(bool);
    void updatePlainTextEdit(const QString &);
    void dataCollected(fcube _samples, fmat _thetas, Hzmat _freqs);

public slots:
    void samplesFileLocationReceived(QEventLoop *el, QString path);
    void onDOAResultsReceived(float, QVector<QPointF>);
    void onTableChanged(Hz, int);
    void updateDlg(const DFSettings *);

private slots:
    void newFile();
    void importFile();
};

#endif // FIELD_TEST_DLG_H
