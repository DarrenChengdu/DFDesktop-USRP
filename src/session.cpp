#include "session.h"
#include <QFile>
//#include "globals.h"

Session::Session(QObject *parent) : QObject(parent)
{
    device = new USRP_X310_2CH();

    settings = new DFSettings();

    QString fileName = QString("./presets.xml");

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        MY_WARNING << "Failed to open file " << fileName;
    }

//    if (!reader->read(&file))
//    {
//        MYOUT_W << "Presets of DFServer parse error in file " << fileName;
//    }

//    algorithm = getInstance();
    dataSource = new DataSource;
    tableSource = new TableSource(NUM_ANTENNAS, this);
}

Session::~Session()
{
    delete []device;
    delete []settings;
    delete []algorithm;
    delete []dataSource;
    delete []tableSource;
}



