#ifndef SIMPLETABLE_H
#define SIMPLETABLE_H

#include <QWidget>
#include <QStandardItemModel>
#include "df_settings.h"
#include "globals.h"

class QTableView;
class QComboBox;
class QLineEdit;

class SimpleTable : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleTable(QAbstractItemModel *model, QWidget *parent = nullptr);
    void setColumnWidth(int column, int width);

private:
    QTableView *tableView;
    QComboBox *azimCombo;
    QLineEdit *offsetLine;

signals:
    void freqOffsetChanged(int);
    void dataCorrectedByCAL(bool);
    void azimIndexChanged(int);
private slots:
    void slotCurrentAzimChanged(int);
public slots:
    void setSamplingAzim(QStringList);
    void updateTable(const DFSettings *);
};

#endif // SIMPLETABLE_H
