#include "browse_dlg.h"
#include "newfiledialog.h"
#include <QtWidgets>

FileBrowseDlg::FileBrowseDlg(QWidget *parent) : QDialog(parent), textEdit(new QPlainTextEdit)
{
    //    setFixedSize(320, 480);
    QPushButton *browseButton = new QPushButton(tr("&Browse..."), this);
    connect(browseButton, &QAbstractButton::clicked, this, &FileBrowseDlg::browse);

    directoryComboBox = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
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

QComboBox *FileBrowseDlg::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void FileBrowseDlg::browse()
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
    else {
        return;
    }

    // show the file information in dialog
    QFile file_handle;
    file_handle.setFileName(path);
    file_handle.open(QIODevice::ReadOnly);
    file_handle.seek(20);

    ArrayType type;
    file_handle.read((char *)&type, sizeof(ArrayType));

    textEdit->clear();

    if (type == ArrayType_Circle)
    {
        circular_array_header header;

        file_handle.seek(0);
        file_handle.read((char *)&header, sizeof(circular_array_header));

        Hzvec freqs;

        for (int n = 0; n < header.nLayers; n++)
        {
            Hzvec freqs_layer = regspace<Hzvec>(header.freq_start[n],header.freq_step[n],header.freq_stop[n]);
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
                                      .arg(header.freq_start[n]).arg(header.freq_step[n]).arg(header.freq_stop[n]));
        }
    }

    file_handle.close();
}
