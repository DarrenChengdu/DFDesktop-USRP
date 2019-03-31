#ifndef DATA_COLLECTION_DLG_H
#define DATA_COLLECTION_DLG_H

#include <atomic>
#include <thread>
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include "session.h"
#include "widgets/dock_panel.h"
#include "widgets/entry_widgets.h"
#include "newfiledialog.h"


class CollectingDlg : public QMainWindow
{
    Q_OBJECT
public:
    explicit CollectingDlg(Session *sPtr, QWidget *parent = 0);

private:
    void createMenuBar();
    void StartStreaming();
    void StopStreaming();
    void CollectingThread();
    void InitFrequencyList(Frequency, Frequency);

//    NewFileDialog *newFileDlg;
    DockPage *page;
    Session *session_ptr;
    CheckBoxEntry *single;
    FrequencyEntry *start;
    FrequencyEntry *stop;
    ComboEntry *theta;
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

signals:
    void updatePlainTextEdit(const QString &);
    void dataCollected(fcube _samples, fmat _thetas, Hzmat _freqs);

public slots:
    void samplesFileLocationReceived(QString path);

private slots:
    void newFile();
    void importFile();
};

#endif // DATA_COLLECTION_DLG_H
