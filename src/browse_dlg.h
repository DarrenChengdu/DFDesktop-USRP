#ifndef BROWSE_DLG_H
#define BROWSE_DLG_H

#include <QDialog>

class QComboBox;
class QPlainTextEdit;

class FileBrowseDlg : public QDialog
{
    Q_OBJECT
public:
    explicit FileBrowseDlg(QWidget *parent = 0);
    ~FileBrowseDlg() {}
    QString getFilePath() const {return path;}
private:
    void browse();
    QComboBox *createComboBox(const QString &text);
    QPlainTextEdit *textEdit;
    QComboBox *directoryComboBox;
    QString path;
};

#endif // BROWSE_DLG_H
