#include "samples_importing_dialog.h"
#include <QtWidgets>
#include <thread>
#include "newfiledialog.h"
#include "widgets/progress_dialog.h"

SamplesImportingDialog::SamplesImportingDialog(Session *sPtr, QWidget *parent) : QDialog(parent),
    textEdit(new QPlainTextEdit), session_ptr(sPtr)
{
//    setFixedSize(320, 480);
    QPushButton *browseButton = new QPushButton(tr("&Browse..."), this);
    connect(browseButton, &QAbstractButton::clicked, this, &SamplesImportingDialog::browse);

    directoryComboBox = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, [=]()
    {
        SHProgressDialog pd(tr("Import Samples ..."), this);
        pd.show();

        QEventLoop el;
        std::thread t = std::thread(&SamplesImportingDialog::import,
                                    this,
                                    &el);
        el.exec();
        if(t.joinable()) {
            t.join();
        }

        QMessageBox::information(this, tr("Samples Importing"), tr("Samples imported!"));
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QGridLayout *layout_grid = new QGridLayout;
    layout_grid->addWidget(new QLabel(tr("In directory:")), 0, 0);
    layout_grid->addWidget(directoryComboBox, 0, 1);
    layout_grid->addWidget(browseButton, 0, 2);
    layout_grid->addWidget(textEdit, 1, 0, 1, 3);

    QVBoxLayout *layout_v = new QVBoxLayout(this);
    layout_v->addLayout(layout_grid);
    layout_v->addWidget(buttonBox);

    setWindowTitle(tr("Samples File Importing"));
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 3);
}

QComboBox *SamplesImportingDialog::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void SamplesImportingDialog::browse()
{
    path = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Samples Files"),
                                                                  QDir::currentPath(),
                                                                  "Samples File(*.dat)"));

    if (!path.isEmpty())
    {
        if (directoryComboBox->findText(path) == -1)
            directoryComboBox->addItem(path);

        directoryComboBox->setCurrentIndex(directoryComboBox->findText(path));
    }

    // show the file information in dialog
    QFile file_handle;
    file_handle.setFileName(path);
    file_handle.open(QIODevice::ReadOnly);
    file_handle.seek(20);

    ARRAY_TYPE type;
    file_handle.read((char *)&type, sizeof(ARRAY_TYPE));

    textEdit->clear();

    if (type == ARRAY_CIRCLE)
    {
        circular_array_header header;

        file_handle.seek(0);
        file_handle.read((char *)&header, sizeof(circular_array_header));

        Hzvec freqs;

        for (int n = 0; n < header.nLayers; n++)
        {
            Hzvec freqs_layer = regspace<Hzvec>(header.freqs_start[n],header.freqs_step[n],header.freqs_stop[n]);
            freqs = join_cols(freqs, freqs_layer);
        }

        int Nf = freqs.size();

        fvec thetas = regspace<fvec>(header.theta_start,header.theta_step,header.theta_stop);
        int Nt = thetas.size();

        textEdit->appendPlainText(tr("The file's creation time: ")
                                  +QDateTime::fromSecsSinceEpoch(header.time_created).toString("yyyy-MM-dd hh:mm:ss"));
        textEdit->appendPlainText(tr("The last modified time: ")
                                  +QDateTime::fromSecsSinceEpoch(header.time_modified).toString("yyyy-MM-dd hh:mm:ss"));
        textEdit->appendPlainText(tr("Array Type: Circular"));
        textEdit->appendPlainText(QString(tr("Antennas Count: %1")).arg(header.nAnts));
        textEdit->appendPlainText(QString(tr("Theta_Start: %1 deg.")).arg(header.theta_start));
        textEdit->appendPlainText(QString(tr("Theta_Step: %1 deg.")).arg(header.theta_step));
        textEdit->appendPlainText(QString(tr("Theta_Stop: %1 deg.")).arg(header.theta_stop));
        textEdit->appendPlainText(QString(tr("Layers Count: %1")).arg(header.nLayers));

        for (int n = 0; n < header.nLayers; n++)
        {
            textEdit->appendPlainText(QString(tr("Layer %1, radius = %2")).arg(n+1).arg(header.radius[n]));
            textEdit->appendPlainText(QString(tr("Freq_Start: %1 Hz; Freq_Step: %2 Hz; Freq_Stop: %3 Hz"))
                                      .arg(header.freqs_start[n]).arg(header.freqs_step[n]).arg(header.freqs_stop[n]));
        }
    }


    file_handle.close();
}

void SamplesImportingDialog::import(QEventLoop *el)
{
    QFile file_handle;
    file_handle.setFileName(path);
    file_handle.open(QIODevice::ReadOnly);
    file_handle.seek(20);

    ARRAY_TYPE type;
    file_handle.read((char *)&type, sizeof(ARRAY_TYPE));

    if (type == ARRAY_CIRCLE)
    {
        circular_array_header header;

        file_handle.seek(0);
        file_handle.read((char *)&header, sizeof(circular_array_header));

        Hzvec freqs;

        for (int n = 0; n < header.nLayers; n++)
        {
            Hzvec freqs_layer = regspace<Hzvec>(header.freqs_start[n],header.freqs_step[n],header.freqs_stop[n]);
            header.freqs_stop[n] = freqs_layer(freqs_layer.size()-1);
            freqs = join_cols(freqs, freqs_layer);
        }

        int Nf = freqs.size();

        fvec thetas = regspace<fvec>(header.theta_start,header.theta_step,header.theta_stop);
        int Nt = thetas.size();
        header.theta_stop = thetas(Nt-1);

        fcube samples(header.nAnts*2, Nf, Nt);

        float current_theta;
        Hz current_freq;

        for (int m = 0; m < Nt; m++)
        {
            for (int n = 0; n < Nf; n++)
            {
                file_handle.read((char *)&current_theta, sizeof(float));
                file_handle.read((char *)&current_freq, sizeof(Hz));

                for (int k = 0; k < header.nAnts*2; k++)
                {
                    float val = 0.0;
                    file_handle.read((char *)&val, sizeof(float));
                    samples(k,n,m) = val;
                }
            }
        }

        session_ptr->algorithm->ConfigCircularArray(header.nAnts, header.nLayers, header.radius,
                                                    header.freqs_start, header.freqs_step, header.freqs_stop,
                                                    header.theta_start, header.theta_step, header.theta_stop);

        emit samplesImported(samples); // send to mainwindow
    }

    file_handle.close();

    emit samplesFileLocation(path); // send to data collection dialog

    while(!el->isRunning()) {
        MSleep(1);
    }

    el->exit();
}


