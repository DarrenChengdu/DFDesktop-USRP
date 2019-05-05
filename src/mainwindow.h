#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QTableView>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QStandardItemModel>
#include <thread>
#include "desk_panel.h"
#include "session.h"
#include "factory.h"
#include "sweep_central.h"
//#include "compass_panel.h"
#include "newfiledialog.h"
#include "samples_importing_dialog.h"
#include "data_collecting_dlg.h"
#include "detection_dlg.h"
#include "field_test_dlg.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QGridLayout>

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QGridLayout *layout;
    std::atomic<int> updatedCount;
    QList<QGraphicsSimpleTextItem *> markerTextList;
    QList<QChart *> chartList;
    QList<QLineSeries *> seriesList;
    QList<QScatterSeries *> markerList;    

    QWidget *central_widget;
    DeskPanel *right_panel;
    SamplesImportingDialog *importing_dlg;
    CollectingDlg *collecting_dlg;
    FieldTestDlg *field_dlg;
    DetectDlg *detect_dlg;

    Session *session;
    SweepCentral *DFSweep;

    // plots schedule
    int num_plots;
    int num_rows;

    void setupToolBar();
    void setupMenuBar();
    void setupChartsAndTable();
    void connectDeviceUponOpen();
    void OpenDevice();
    void OpenDeviceInThread(QEventLoop *el, Device *device);
    void deviceConnected(bool success);

private slots:
    void onSceneChanged();
    void newFile();
    void importSamplesFile();

signals:

};

#endif // MAINWINDOW_H
