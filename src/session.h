#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "seven_channels.h"
#include "df_settings.h"
#include "globals.h"
#include "libci.h"
#include "datasource.h"
#include "tablesource.h"

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = 0);
    ~Session();

public:
    Device *device;
    // Sweep/Real-time related
    DFSettings *settings;
    IAlgorithmCI *algorithm;
    DataSource *dataSource;
    TableSource *tableSource;

signals:

};

#endif // SESSION_H
