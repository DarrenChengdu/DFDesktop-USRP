#include "simpletable.h"
#include <QtWidgets>

SimpleTable::SimpleTable(QAbstractItemModel *model, QWidget *parent) : QWidget(parent)
{
    QLabel *lbl_freq = new QLabel(tr("Frequency Observ. Index"));
    QLabel *lbl_use_cal = new QLabel(tr("Adjusted by CAL:"));
    QLabel *lbl_azim = new QLabel(tr("Azim. (deg.):"));

    QCheckBox *use_cal = new QCheckBox;

    connect(use_cal, &QCheckBox::clicked,
            this, [=]()
    {
        emit dataCorrectedByCAL(use_cal->isChecked());
    });

    offsetLine = new QLineEdit;
    offsetLine->setText(QString("%1").arg(0));

    connect(offsetLine, &QLineEdit::editingFinished,
            this, [=]() {
        int offset = offsetLine->text().toInt();
        emit freqOffsetChanged(offset);
    });

    azimCombo = new QComboBox;
    azimCombo->setEnabled(false);

    connect(azimCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCurrentAzimChanged(int)));

    tableView = new QTableView;
    tableView->setModel(model);
    tableView->verticalHeader()->setVisible(false);
    tableView->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    QHBoxLayout *layout_h = new QHBoxLayout;
    layout_h->addWidget(lbl_freq);
    layout_h->addWidget(offsetLine);
    layout_h->addSpacing(25);
    layout_h->addWidget(lbl_use_cal);
    layout_h->addWidget(use_cal);
    layout_h->addSpacing(25);
    layout_h->addWidget(lbl_azim);
    layout_h->addWidget(azimCombo);

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
    if (!azimCombo->isEnabled())
        azimCombo->setEnabled(true);

    azimCombo->clear();
    azimCombo->insertItems(0, _list);
    azimCombo->setCurrentIndex(0);
}

void SimpleTable::slotCurrentAzimChanged(int ind)
{
    emit azimIndexChanged(ind);
}

void SimpleTable::updateTable(const DFSettings *s)
{
    offsetLine->setText(QString("%1").arg(s->FreqObservIndex()));
}
