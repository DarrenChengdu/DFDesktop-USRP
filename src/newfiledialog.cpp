#include "newfiledialog.h"
#include <QtWidgets>
#include "widgets/progress_dialog.h"

const unsigned short samples_file_signature = 0xDF20;
const unsigned short samples_file_version = 0x1;

NewFileDialog::NewFileDialog(QWidget *parent) : QDialog(parent)
{
    DockPanel *panel = new DockPanel("", this);
    page = new DockPage(tr("New File Generation"));

//    algorithm = getInstance();

    array_type = new ComboEntry(tr("Array Type"));
    QStringList type_str;
    type_str << "Circular";
    array_type->setComboText(type_str);
    array_type->setComboIndex(0);

    ants_count = new NumericEntry(tr("Ants Count"), 7, tr(""));
    array_dia_0 = new NumericEntry(tr("Array Dia(1)"), 3.0, tr(""));
    array_dia_1 = new NumericEntry(tr("Array Dia(2)"), 0.8, tr(""));
    array_dia_2 = new NumericEntry(tr("Array Dia(3)"), 0.8, tr(""));
    start_freq_0 = new FrequencyEntry(tr("Start Freq(1)"), 30000000);
    step_freq_0 = new FrequencyEntry(tr("Step Freq(1)"), 1000000);
    stop_freq_0 = new FrequencyEntry(tr("Stop Freq(1)"), 400000000);
    start_freq_1 = new FrequencyEntry(tr("Start Freq(2)"), 401000000);
    step_freq_1 = new FrequencyEntry(tr("Step Freq(2)"), 1000000);
    stop_freq_1 = new FrequencyEntry(tr("Stop Freq(2)"), 1350000000);
    start_freq_2 = new FrequencyEntry(tr("Start Freq(3)"), 1351000000);
    step_freq_2 = new FrequencyEntry(tr("Step Freq(3)"), 1000000);
    stop_freq_2 = new FrequencyEntry(tr("Stop Freq(3)"), 3000000000);

    start_azim = new NumericEntry(tr("Start Azim."), 0, tr("deg."));
    step_azim = new NumericEntry(tr("Step Azim."), 5, tr("deg."));
    stop_azim = new NumericEntry(tr("Stop Azim."), 355, tr("deg."));

    array_layers = new ComboEntry(tr("Array Layers"));
    QStringList layers_str;
    layers_str << "1" << "2" << "3";
    array_layers->setComboText(layers_str);
    array_layers->setComboIndex(2);

    connect(array_layers, &ComboEntry::comboIndexChanged,
            this, [=](int ind)
    {
        if (ind == 0)
        {
            array_dia_0->setEnabled(true);
            start_freq_0->setEnabled(true);
            step_freq_0->setEnabled(true);
            stop_freq_0->setEnabled(true);
            array_dia_1->setEnabled(false);
            start_freq_1->setEnabled(false);
            step_freq_1->setEnabled(false);
            stop_freq_1->setEnabled(false);
            array_dia_2->setEnabled(false);
            start_freq_2->setEnabled(false);
            step_freq_2->setEnabled(false);
            stop_freq_2->setEnabled(false);
        }

        if (ind == 1)
        {
            array_dia_0->setEnabled(true);
            start_freq_0->setEnabled(true);
            step_freq_0->setEnabled(true);
            stop_freq_0->setEnabled(true);
            array_dia_1->setEnabled(true);
            start_freq_1->setEnabled(true);
            step_freq_1->setEnabled(true);
            stop_freq_1->setEnabled(true);
            array_dia_2->setEnabled(false);
            start_freq_2->setEnabled(false);
            step_freq_2->setEnabled(false);
            stop_freq_2->setEnabled(false);
        }

        if (ind == 2)
        {
            array_dia_0->setEnabled(true);
            start_freq_0->setEnabled(true);
            step_freq_0->setEnabled(true);
            stop_freq_0->setEnabled(true);
            array_dia_1->setEnabled(true);
            start_freq_1->setEnabled(true);
            step_freq_1->setEnabled(true);
            stop_freq_1->setEnabled(true);
            array_dia_2->setEnabled(true);
            start_freq_2->setEnabled(true);
            step_freq_2->setEnabled(true);
            stop_freq_2->setEnabled(true);
        }
    });


    use_theor = new CheckBoxEntry(tr("Theory Data"));
    use_theor->SetChecked(true);

    apply_or_close = new DualButtonEntry(tr("Apply"), tr("Close"));
    apply_or_close->LeftButton()->setFocusPolicy(Qt::NoFocus);
    apply_or_close->RightButton()->setFocusPolicy(Qt::NoFocus);

    connect(apply_or_close, &DualButtonEntry::leftPressed,
            this, [=]()
    {
        SHProgressDialog pd(tr("Setup New Table ..."), this);
        pd.show();

        QEventLoop el;
        std::thread t = std::thread(&NewFileDialog::setupNewTable,
                                    this,
                                    &el);
        el.exec();
        if(t.joinable()) {
            t.join();
        }

        QMessageBox::information(this, tr("New File"), tr("New file saved!"));
    });

    connect(apply_or_close, &DualButtonEntry::rightPressed,
            this, [=]()
    {
        reject();
    });

    page->AddWidget(array_type);
    page->AddWidget(ants_count);
    page->AddWidget(start_azim);
    page->AddWidget(step_azim);
    page->AddWidget(stop_azim);
    page->AddWidget(array_layers);
    page->AddWidget(array_dia_0);
    page->AddWidget(start_freq_0);
    page->AddWidget(step_freq_0);
    page->AddWidget(stop_freq_0);
    page->AddWidget(array_dia_1);
    page->AddWidget(start_freq_1);
    page->AddWidget(step_freq_1);
    page->AddWidget(stop_freq_1);
    page->AddWidget(array_dia_2);
    page->AddWidget(start_freq_2);
    page->AddWidget(step_freq_2);
    page->AddWidget(stop_freq_2);

    page->AddWidget(use_theor);
    page->AddWidget(new SpacerEntry);
    page->AddWidget(apply_or_close);

    panel->AppendPage(page);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(panel);

    setLayout(mainLayout);

    page->SetOpen(true);
    int height = page->GetTotalHeight();
    setFixedHeight(height + 80);
}

int NewFileDialog::exec()
{
    QTimer m_dataUpdater;
    m_dataUpdater.setInterval(10);
    m_dataUpdater.setSingleShot(true);
    QObject::connect(&m_dataUpdater, &QTimer::timeout,
                     this, &NewFileDialog::togglePage);
    m_dataUpdater.start();

    return QDialog::exec();
}

void NewFileDialog::togglePage() {
    page->SetOpen(false);
    page->SetOpen(true);
}

void NewFileDialog::setupNewTable(QEventLoop *el)
{
    short type = array_type->currentIndex();

    if(file_handle.isOpen()) return;

    QString file_name = "new_table.dat";

    file_handle.setFileName(file_name);
    file_handle.open(QIODevice::WriteOnly);
    file_handle.seek(0);

    if (type == 0)
    {
        circular_array_header header;
        header.signature = samples_file_signature;
        header.version = samples_file_version;

        qint64 t0 = QDateTime::currentSecsSinceEpoch();
        header.time_created = t0;
        header.time_modified = t0;
        header.type = ArrayType_Circle;
        header.nAnts = ants_count->GetValue();
        header.theta_start = start_azim->GetValue();
        header.theta_step = step_azim->GetValue();
        header.theta_stop = stop_azim->GetValue();
        header.nLayers = array_layers->currentIndex() + 1;
        header.radius[0] = array_dia_0->GetValue()/2;
        header.radius[1] = array_dia_1->GetValue()/2;
        header.radius[2] = array_dia_2->GetValue()/2;
        header.freq_start[0] = start_freq_0->GetFrequency();
        header.freq_start[1] = start_freq_1->GetFrequency();
        header.freq_start[2] = start_freq_2->GetFrequency();
        header.freq_step[0] = step_freq_0->GetFrequency();
        header.freq_step[1] = step_freq_1->GetFrequency();
        header.freq_step[2] = step_freq_2->GetFrequency();
        header.freq_stop[0] = stop_freq_0->GetFrequency();
        header.freq_stop[1] = stop_freq_1->GetFrequency();
        header.freq_stop[2] = stop_freq_2->GetFrequency();

        algorithm->ConfigCircularArray(header.nAnts, header.nLayers, header.radius,
                                       header.freq_start, header.freq_step, header.freq_stop,
                                       header.theta_start, header.theta_step, header.theta_stop);

        file_handle.write((char*)&header, sizeof(circular_array_header));

        fvec thetas = regspace<fvec>(header.theta_start, header.theta_step, header.theta_stop);

        Hzvec freqs;

        for (int n = 0; n < header.nLayers; n++)
        {
            Hzvec freqs_layer = regspace<Hzvec>(header.freq_start[n],header.freq_step[n],header.freq_stop[n]);
            header.freq_stop[n] = freqs_layer(freqs_layer.size()-1);
            freqs = join_cols(freqs, freqs_layer);
        }

        for (int i = 0; i < thetas.size(); i++)
        {
            float theta = thetas(i);

            for (int j = 0; j < freqs.size(); j++)
            {
                Hz freq = freqs(j);

                const float *phases_theor = algorithm->GetTheorPHATabl(1, &freq, 1, &theta);

                file_handle.write((char *)&theta, sizeof(float));
                file_handle.write((char *)&freq, sizeof(Hz));

                for (int k = 0; k < header.nAnts*2; k++)
                {
                    if (k < header.nAnts) {
                        float amp = -20.0;
                        file_handle.write((char *)&amp, sizeof(float));
                    }
                    else {
                        float pha = 0.0;

                        if (use_theor->IsChecked()) {
                            pha = phases_theor[k-header.nAnts];
                        }

                        file_handle.write((char *)&pha, sizeof(float));
                    }
                }

                delete []phases_theor;
            }

        }
    }

    file_handle.close();

    while(!el->isRunning()) {
        MSleep(1);
    }
    el->exit();
}
