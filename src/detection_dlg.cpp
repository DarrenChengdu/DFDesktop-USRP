#include "detection_dlg.h"
#include <QtWidgets>

DetectDlg::DetectDlg(Session *sPtr, QWidget *parent) : session_ptr(sPtr), QDialog(parent)
{
    panel = new DockPanel(tr("Configuration"), this);
    page = new DockPage(tr("Configuration"));

    start = new FrequencyEntry(tr("Frequency Start"), 100000000);

    stop = new FrequencyEntry(tr("Frequency Stop"), 500000000);

    dual_btn = new DualButtonEntry(tr("Run"), tr("Cancel"));
    dual_btn->LeftButton()->setFocusPolicy(Qt::NoFocus);
    dual_btn->RightButton()->setFocusPolicy(Qt::NoFocus);

    connect(dual_btn, &DualButtonEntry::leftPressed, this, [=]()
    {
        dual_btn->LeftButton()->setEnabled(false);
        dual_btn->RightButton()->setEnabled(true);

        StartStreaming();
    });

    connect(dual_btn, &DualButtonEntry::rightPressed, this, [=]()
    {
        StopStreaming();

        dual_btn->RightButton()->setEnabled(false);
        dual_btn->LeftButton()->setEnabled(true);
    });

    page->AddWidget(start);
    page->AddWidget(stop);
    page->AddWidget(dual_btn);

    panel->AppendPage(page);

    textEdit = new QPlainTextEdit;

    connect(this, &DetectDlg::updatePlainTextEdit,
            textEdit, &QPlainTextEdit::appendPlainText);

    table_model = new QStandardItemModel;
    QStringList list_h;
    list_h << "Center Freq.[MHz]" << "Bandwidth[Hz]"  << "Bearing[deg.]" << "Start Time" << "Last Seen On Air"
           << "Stop Time" << "Duration[d hh:mm:ss:f]" << "Mean Peak Power" << "Mean Peak SNR";
    table_model->setHorizontalHeaderLabels(list_h);

    tableView = new QTableView;
    tableView->setModel(table_model);
    tableView->verticalHeader()->setVisible(false);
    tableView->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    for (int col = 0; col < list_h.length(); ++col)
    {
        tableView->setColumnWidth(col, 120);
    }

    waterfall = new WaterfallPlot(this);
    waterfall->SetMinMax(0,360);

    QVBoxLayout *layout_v = new QVBoxLayout;
    layout_v->addWidget(waterfall);
    layout_v->addWidget(tableView);

    QHBoxLayout *layout_h = new QHBoxLayout(this);
    layout_h->addLayout(layout_v);
    layout_h->addWidget(panel);

    setWindowTitle(tr("Detection Dialog"));

//    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
//    resize(screenGeometry.width() / 2, screenGeometry.height() / 3);
}

void DetectDlg::togglePage()
{
    page->SetOpen(false);
    page->SetOpen(true);
}

void DetectDlg::StartStreaming()
{
    running = true;
    thread_handle = std::thread(&DetectDlg::DetectThread, this);
}

void DetectDlg::StopStreaming()
{
    if(thread_handle.joinable())
    {
        running = false;
        thread_handle.join();
        //        cout << "SweepCentral::StopStreaming" << endl;
    }
}

void DetectDlg::DetectThread()
{

    while (running)
    {
        snapshot_mtx.lock();
        if (phases_q.read_available() > 0 && amplitudes_q.read_available() > 0)
        {
            mat amplitudes_tmp;
            amplitudes_q.pop(amplitudes_tmp);
            snapshot_mtx.unlock();


        }
        else
        {
            snapshot_mtx.unlock();
            MSleep(10);
        }
    }
}

