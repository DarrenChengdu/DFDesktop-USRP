#ifndef DETECTION_DLG_H
#define DETECTION_DLG_H

#include <QDialog>
#include <atomic>
#include <thread>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include "session.h"
#include "waterfall_plot.h"
#include "widgets/dock_panel.h"
#include "widgets/entry_widgets.h"

class QStandardItemModel;
class QTableView;

class DetectDlg : public QDialog
{
    Q_OBJECT
public:
    explicit DetectDlg(Session *sPtr, QWidget *parent = 0);
    void togglePage();

private:
    void StartStreaming();
    void StopStreaming();
    void DetectThread();

    DockPanel *panel;
    DockPage *page;
    Session *session_ptr;
    FrequencyEntry *start;
    FrequencyEntry *stop;
    DualButtonEntry *dual_btn;
    QPlainTextEdit *textEdit;
    QStandardItemModel *table_model;
    QTableView *tableView;
    WaterfallPlot *waterfall;

    QString samples_location;

    std::thread thread_handle;
    std::atomic<bool> running;

signals:
    void updatePlainTextEdit(const QString &);

private slots:

public slots:

};

#endif // DETECTION_DLG_H
