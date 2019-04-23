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
    QComboBox *combo_azim;

signals:
    void dataCorrectedByCAL(bool);
    void azimIndexChanged(int);

private slots:
    void slotCurrentAzimChanged(int);

public slots:
    void setSamplingAzim(QStringList);
};

#endif // SIMPLETABLE_H
