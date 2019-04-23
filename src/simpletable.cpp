#include "simpletable.h"
#include <QtWidgets>

SimpleTable::SimpleTable(QAbstractItemModel *model, QWidget *parent) : QWidget(parent)
{
    QLabel *lbl_use_cal = new QLabel(tr("Adjusted by CAL:"));
    QLabel *lbl_azim = new QLabel(tr("Azim. (deg.):"));

    QCheckBox *use_cal = new QCheckBox;

    connect(use_cal, &QCheckBox::clicked,
            this, [=]()
    {
        emit dataCorrectedByCAL(use_cal->isChecked());
    });

    combo_azim = new QComboBox;
    combo_azim->setEnabled(false);

    connect(combo_azim, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCurrentAzimChanged(int)));

    tableView = new QTableView;
    tableView->setModel(model);
    tableView->verticalHeader()->setVisible(false);
    tableView->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    QHBoxLayout *layout_h = new QHBoxLayout;
    layout_h->addWidget(lbl_use_cal);
    layout_h->addWidget(use_cal);
    layout_h->addSpacing(25);
    layout_h->addWidget(lbl_azim);
    layout_h->addWidget(combo_azim);

    QVBoxLayout *layout_v = new QVBoxLayout;
    layout_v->addLayout(layout_h);
    layout_v->addWidget(tableView);
    setLayout(layout_v);
}

void SimpleTable::setColumnWidth(int column, int width)
{
    tableView->setColumnWidth(column, width);
}

void SimpleTable::setSamplingAzim(QStringList _list)
{
    if (!combo_azim->isEnabled())
        combo_azim->setEnabled(true);

    combo_azim->clear();
    combo_azim->insertItems(0, _list);
    combo_azim->setCurrentIndex(0);
}

void SimpleTable::slotCurrentAzimChanged(int ind)
{
    emit azimIndexChanged(ind);
}

