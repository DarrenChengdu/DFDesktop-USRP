#include "session.h"
#include <QMessageBox>

QString Session::title;

Session::Session()
{
    sweep_settings = new SweepSettings();

    isInPlaybackMode = false;
}

Session::~Session()
{
    delete sweep_settings;
}

void Session::LoadDefaults()
{
    sweep_settings->LoadDefaults();
    sweep_settings->EmitUpdated();
}

void Session::LoadPreset(int p)
{
    QString fileName;
    fileName.sprintf("Preset%d", p);
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "SignalHound",
                       fileName);

    sweep_settings->Load(settings);
}

void Session::SavePreset(int p)
{  
    QString fileName;
    fileName.sprintf("Preset%d", p);
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "SignalHound",
                       fileName);

    sweep_settings->Save(settings);
}

void Session::SetTitle(const QString &new_title)
{
    title = new_title;

    if(title.size() > MAX_TITLE_LEN) {
        title.resize(MAX_TITLE_LEN);
    }
}
