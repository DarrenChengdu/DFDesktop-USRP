#ifndef SAMPLES_IMPORTING_DIALOG_H
#define SAMPLES_IMPORTING_DIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QFile>
#include <QComboBox>
#include "widgets/entry_widgets.h"
#include "widgets/dock_panel.h"
#include "globals.h"
#include "session.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QEventLoop;
QT_END_NAMESPACE

class SamplesImportingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SamplesImportingDialog(Session *sPtr, QWidget *parent = 0);

private:
    void import(QEventLoop *el);
    QPlainTextEdit *textEdit;
    QComboBox *directoryComboBox;
    QComboBox *createComboBox(const QString &text);
    QDialogButtonBox *buttonBox;
    QString path;

    Session *session_ptr;

signals:
    void samplesImported(fcube);
    void samplesFileLocation(QString);

private slots:
    void browse();


public slots:
};

#endif // SAMPLES_IMPORTING_DIALOG_H
