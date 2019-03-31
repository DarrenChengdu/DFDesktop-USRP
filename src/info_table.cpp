#include "info_table.h"

InfoTable::InfoTable(Session *sPtr, QWidget *parent) : session_ptr(sPtr), QWidget(parent)
  , len(CHANNELS_COUNT), offset(FFT_LENGTH/2), d_timerId(-1), samples_loaded(false), atten_rf(0)
{
    amplitudes.set_size(len); amplitudes.fill(-70);
    phases.set_size(len); phases.fill(0);

    QLabel *lbl_freq = new QLabel(tr("Offset:"));
    QLabel *lbl_azim = new QLabel(tr("Azimuth(deg.):"));
    QLabel *lbl_use_cal = new QLabel(tr("Adjusted by CAL:"));

    use_cal = new QCheckBox();
    use_cal->setChecked(true);
    calibrated = true;

    connect(use_cal, &QCheckBox::clicked,
            this, [&](bool sta)
    {
        calibrated = sta;
    });

    samples_azim = new QComboBox;
    vec thetas_v = regspace<vec>(0, 5, 355);

    samples_azim->clear();
    QStringList azim_str;

    for (int n = 0; n < thetas_v.size(); n++)
    {
        azim_str << QString("%1").arg(thetas_v(n));
    }

    samples_azim->insertItems(0, azim_str);
    samples_azim->setCurrentIndex(0);

    freq_index = new QLineEdit;
    freq_index->setText(QString::number(offset));

    connect(freq_index, &QLineEdit::editingFinished,
            this, &InfoTable::onFreqIndexEditingFinished);

    table_model = new QStandardItemModel;
    QStringList list_h;
    list_h << "Type" << "RT" << "Sampl." << "Theor."
           << "Type" << "RT" << "Sampl." << "RF Atten.";
    table_model->setHorizontalHeaderLabels(list_h);

    for (int row = 0; row < len; ++row)
    {
        QStandardItem *newItem = new QStandardItem(QString("P%1").arg(row+1));
        newItem->setTextAlignment(Qt::AlignCenter);
        newItem->setBackground(QBrush(QColor("darkGray")));
        table_model->setItem(row, 0, newItem);

        QStandardItem *newItem2 = new QStandardItem(QString("A%1").arg(row+1));
        newItem2->setTextAlignment(Qt::AlignCenter);
        newItem2->setBackground(QBrush(QColor("darkGray")));
        table_model->setItem(row, 4, newItem2);
    }

    tableView = new QTableView;
    tableView->setModel(table_model);
    tableView->verticalHeader()->setVisible(false);
    tableView->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    for (int col = 0; col < list_h.length(); ++col)
    {
        tableView->setColumnWidth(col, 60);
    }

    QHBoxLayout *layout_h = new QHBoxLayout;
    layout_h->addWidget(lbl_freq);
    layout_h->addWidget(freq_index);
    layout_h->addSpacing(25);
//    layout_h->addWidget(new QWidget);
    layout_h->addWidget(lbl_use_cal);
    layout_h->addWidget(use_cal);
//    layout_h->addWidget(new QWidget);
    layout_h->addSpacing(25);
    layout_h->addWidget(lbl_azim);
    layout_h->addWidget(samples_azim);

    QVBoxLayout *layout_v = new QVBoxLayout;
    layout_v->addLayout(layout_h);
    layout_v->addWidget(tableView);
    setLayout(layout_v);

    init();

    d_timerId = startTimer( ceil(1000/5.0) );

    setFixedHeight(340);
}

void InfoTable::init()
{

}

void InfoTable::onFreqIndexEditingFinished()
{
    mutex.lock();
    offset = freq_index->text().toUInt();
    //        df_lib::clamp<int>(offset, 0, FFT_LENGTH-1);
    freq_index->setText(QString::number(offset));
    mutex.unlock();
}

void InfoTable::slotTableUpdated(DataFactory *factory)
{
    mutex.lock();

    for (int n = 0; n < len; n++)
    {
        amplitudes(n) = factory->amplitudes(offset, n);
        phases(n) = factory->phases(offset, n);

        if (calibrated)
        {
            amplitudes(n) = amplitudes(n) - factory->amplitudes_cal(offset, n);
            phases(n) = df_lib::angle(phases(n) - factory->phases_cal(offset, n));
        }
    }

    atten_rf = factory->atten_rf(offset/FFT_LENGTH);

    mutex.unlock();
}

void InfoTable::timerEvent( QTimerEvent *e )
{
    if (e->timerId() == d_timerId)
    {
        mutex.lock();
        for (int row = 0; row < len; ++row)
        {
            QStandardItem *newItem = new QStandardItem(QString::number(phases(row), 'f', 1));
            newItem->setTextAlignment(Qt::AlignCenter);
            newItem->setBackground(QBrush(QColor("white")));
            table_model->setItem(row, 1, newItem);

            QStandardItem *newItem2 = new QStandardItem(QString::number(amplitudes(row), 'f', 1));
            newItem2->setTextAlignment(Qt::AlignCenter);
            newItem2->setBackground(QBrush(QColor("white")));
            table_model->setItem(row, 5, newItem2);

            QStandardItem *newItem3 = new QStandardItem(QString("%1 dB").arg(atten_rf));
            newItem3->setTextAlignment(Qt::AlignCenter);
            newItem3->setBackground(QBrush(QColor("white")));
            table_model->setItem(row, 7, newItem3);

            if (samples_loaded)
            {
                Hz bw = session_ptr->settings->BW();
                Hz rbw = session_ptr->settings->RBW();
                current_freq = session_ptr->settings->Center() - bw/2 + offset*rbw;

                int ind_f = session_ptr->algorithm->FreqIndex(current_freq);
                int ind_t = samples_azim->currentIndex();

                QStandardItem *newItem4 = new QStandardItem(QString("%1").arg(phases_sampl(ind_t, row, ind_f)));
                newItem4->setTextAlignment(Qt::AlignCenter);
                newItem4->setBackground(QBrush(QColor("white")));
                table_model->setItem(row, 2, newItem4);

                QStandardItem *newItem5 = new QStandardItem(QString("%1").arg(amplitudes_sampl(ind_t, row, ind_f)));
                newItem5->setTextAlignment(Qt::AlignCenter);
                newItem5->setBackground(QBrush(QColor("white")));
                table_model->setItem(row, 6, newItem5);

                QStandardItem *newItem6 = new QStandardItem(QString("%1").arg(phases_sampl_theor(ind_t, row, ind_f)));
                newItem6->setTextAlignment(Qt::AlignCenter);
                newItem6->setBackground(QBrush(QColor("white")));
                table_model->setItem(row, 3, newItem6);
            }
        }
        mutex.unlock();
    }
}

void InfoTable::setSamplesEnabled(bool en)
{
    if (en)
    {
//        phases_sampl = session_ptr->algorithm->PhaseSamples();
//        phases_sampl_theor = session_ptr->algorithm->TheorPhaseSamples();
//        amplitudes_sampl = session_ptr->algorithm->AmpSamples();

        // thetas updates
        thetas = session_ptr->algorithm->Thetas();

        samples_azim->clear();
        QStringList azim_str;

        for (int n = 0; n < thetas.size(); n++)
        {
            azim_str << QString("%1").arg(thetas(n));
        }

        samples_azim->insertItems(0, azim_str);
        samples_azim->setCurrentIndex(0);
    }

    samples_loaded = en;
}


