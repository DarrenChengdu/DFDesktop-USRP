#include "mainwindow.h"
#include <QApplication>
#include <QFile>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 去除一些不必要的 log 信息
//    if (msg.contains("Unknown property min-width"))
//        return;

    static QMutex mutex;
    mutex.lock();

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString text, message;

    switch (type) {
    case QtDebugMsg:
        text = QString("Debug:");
        message = QString("%1 %2 %3").arg(current_date).arg(text).arg(msg);
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        message = QString("%1 %2 %3").arg(current_date).arg(text).arg(msg);
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        message = QString("%1 %2 %3").arg(current_date).arg(text).arg(msg);
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        message = QString("%1 %2 %3").arg(current_date).arg(text).arg(msg);
        break;
    case QtInfoMsg:
        text = QString("Info:");
        message = QString("%1 %2 %3").arg(current_date).arg(text).arg(msg);
        break;
    default:
        text = QString("Unknown:");
        break;
    }

//    fprintf(stderr, message.toLatin1().data());
//    fprintf(stderr, "\r\n");
    qDebug() << message.toUtf8().data();

    QString logFile = QCoreApplication::applicationDirPath()+"/log.txt";
    QFile file(logFile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    if (file.size() >= 10*1024*1024) // 文件达到 10MB 后先备份再清空
    {
        QFile::remove("log_back.txt");
        file.copy("log_back.txt");
        file.resize(0);
    }

    QTextStream textStream(&file);
    textStream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    Q_INIT_RESOURCE(application);
    QApplication a(argc, argv);

    int fdsa = 1;

    QTranslator trans;
    trans.load("desktop_cn");
    a.installTranslator(&trans);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
