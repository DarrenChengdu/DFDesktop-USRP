#include "mainwindow.h"
#include <QStatusBar>
#include <QMenuBar>
#include <glchartview.h>
#include "simpletable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), updatedCount(0), num_plots(NUM_ANTENNAS), num_rows(3)
{
    setObjectName("MainWindow");
    setWindowTitle("DF-Desktop");    

    session = new Session;
    DFSweep = new SweepCentral(session);

    connect(session->settings, SIGNAL(updated(const DFSettings*)),
            DFSweep, SLOT(settingsChanged(const DFSettings*)));

    right_panel = new DeskPanel(tr("Device Panel"), session, this);
    right_panel->setObjectName("DeviceCtrlPanel");
    addDockWidget(Qt::RightDockWidgetArea, right_panel);

    importing_dlg = new SamplesImportingDialog(session);

    qRegisterMetaType<fcube>("fcube");
    qRegisterMetaType<fmat>("fmat");
    qRegisterMetaType<Hzmat>("Hzmat");

    importing_dlg->hide();

    collecting_dlg = new CollectingDlg(session);
    collecting_dlg->hide();

    connect(importing_dlg, &SamplesImportingDialog::samplesFileLocation,
            collecting_dlg, &CollectingDlg::samplesFileLocationReceived);

    field_dlg = new FieldTestDlg(session);
    field_dlg->hide();

    qRegisterMetaType<QVector<QPointF>>("CorrCurve");
    connect(DFSweep, &SweepCentral::doaResults,
            field_dlg, &FieldTestDlg::onDOAResultsReceived);

    detect_dlg = new DetectDlg(session);
    detect_dlg->hide();

//    CompassPanel *compass = new CompassPanel;

//    table = new InfoTable(session);
//    connect( , &SweepCentral::dataCompleted,
//            table, &InfoTable::slotTableUpdated);

    setupToolBar();
    setupMenuBar();
    setupChartsAndTable();

//    DFSweep->StartStreaming();

    QLabel *fpsLabel = new QLabel;
    session->dataSource->startUpdates(seriesList, markerList, markerTextList, fpsLabel);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    statusBar()->addWidget(fpsLabel);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupToolBar()
{

}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::importSamplesFile);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    QMenu *dataCollectionMenu = menuBar()->addMenu(tr("&Data Collecting"));
    QToolBar *dataCollectionToolBar = addToolBar(tr("Collection"));

    QAction *collectAct = new QAction(QIcon(":/images/cut.png"), tr("Collect"), this);

//    collectAct->setShortcuts(QKeySequence::Cut);
    collectAct->setStatusTip(tr("Data Collecting"));
    connect(collectAct, &QAction::triggered, this, [=]()
    {
        collecting_dlg->show();
        collecting_dlg->resize(800, 600);
    });
    dataCollectionMenu->addAction(collectAct);
    dataCollectionToolBar->addAction(collectAct);

    QMenu *fieldTestMenu = menuBar()->addMenu(tr("Field Test"));
    QToolBar *fieldTestToolBar = addToolBar(tr("Field Test"));

    QAction *fieldAct = new QAction(QIcon(":/images/save.png"), tr("Field Test"), this);
    fieldAct->setStatusTip(tr("Field Test"));
    connect(fieldAct, &QAction::triggered, this, [=]()
    {
        field_dlg->show();
        field_dlg->resize(800, 600);
    });
    fieldTestMenu->addAction(fieldAct);
    fieldTestToolBar->addAction(fieldAct);

    QMenu *detectMenu = menuBar()->addMenu(tr("Detection"));
    QToolBar *detectToolBar = addToolBar(tr("Detection"));

    QAction *detectAct = new QAction(QIcon(":/images/save.png"), tr("Detection"), this);
    detectAct->setStatusTip(tr("DF with Detection"));

    connect(detectAct, &QAction::triggered, this, [=]()
    {
        detect_dlg->show();
        detect_dlg->togglePage();
    });

    detectMenu->addAction(detectAct);
    detectToolBar->addAction(detectAct);
}

void MainWindow::setupChartsAndTable()
{
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    int n = 0;

    for (n = 0; n < num_plots; n++)
    {
        QString title = QString(QObject::tr("Antenna %1")).arg(n+1);

        GLChartView *plot = new GLChartView(title);

        layout->addWidget(plot, n%num_rows, n/num_rows);

        connect(plot->scene(), &QGraphicsScene::changed,
                this, &MainWindow::onSceneChanged);
        connect(DFSweep, &SweepCentral::plotsAxisXUpdated,
                plot, &GLChartView::setAxisXRange);

        seriesList.append(plot->getLineSeries());
        markerList.append(plot->getMarkerSeries());
        markerTextList.append(plot->getMarkerTextItem());
    }

    QStandardItemModel *simpleModel = new QStandardItemModel;
    session->tableSource->setModel(simpleModel);

    QStringList list_h;
    list_h << ""
           << "RT"
           << "CAL"
           << "Stored"
           << ""
           << "RT"
           << "CAL"
           << "Stored";
    simpleModel->setHorizontalHeaderLabels(list_h);

    for (int row = 0; row < num_plots; ++row)
    {
        QStandardItem *newItem = new QStandardItem(QString("P%1").arg(row+1));
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setBackground(QBrush(QColor("darkGray")));
        simpleModel->setItem(row, 0, newItem);

        QStandardItem *newItem2 = new QStandardItem(QString("A%1").arg(row+1));
        newItem2->setTextAlignment(Qt::AlignCenter);
        newItem2->setBackground(QBrush(QColor("darkGray")));
        simpleModel->setItem(row, 4, newItem2);
    }

    SimpleTable *sTable = new SimpleTable(simpleModel);

//    for (int col = 0; col < list_h.size(); col++) {
//        sTable->setColumnWidth(col,75);
//    }
    sTable->setColumnWidth(0, 25);
    sTable->setColumnWidth(4, 25);

    connect(sTable, &SimpleTable::freqOffsetChanged,
            session->settings, &DFSettings::setFreqObervIndex);
    connect(sTable, &SimpleTable::dataCorrectedByCAL,
            session->tableSource, &TableSource::setCALEnabled);
    connect(sTable, &SimpleTable::azimIndexChanged,
            session->tableSource, &TableSource::setAzimuthIndex);
    connect(field_dlg, &FieldTestDlg::setSamplingAzim,
            sTable, &SimpleTable::setSamplingAzim);
    connect(session->settings, &DFSettings::updated,
            sTable, &SimpleTable::updateTable);

    qRegisterMetaType<Hz>("Hz");
    connect(session->tableSource, &TableSource::tableChanged,
            field_dlg, &FieldTestDlg::onTableChanged);

    layout->addWidget(sTable, n%num_rows, n/num_rows);
}

void MainWindow::onSceneChanged()
{
    updatedCount++;
    //    qDebug() << updatedCount;
    if (updatedCount == num_plots) {
        session->dataSource->handleSceneChanged();
        updatedCount = 0;
    }
}

void MainWindow::newFile()
{
    NewFileDialog newDialog;
    newDialog.exec();
}

void MainWindow::importSamplesFile()
{
    importing_dlg->show();
}

