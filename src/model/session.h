#ifndef SESSION_H
#define SESSION_H

#include "sweep_settings.h"
#include "color_prefs.h"
#include "preferences.h"

const int MAX_TITLE_LEN = 127;

class Session : public QObject {
    Q_OBJECT

public:
    Session();
    ~Session();

    void LoadDefaults();
    void LoadPreset(int p);
    void SavePreset(int p);

    static QString GetTitle() { return title; }
    static void SetTitle(const QString &new_title);
    static QString title;

    // Sweep/Real-time related
    SweepSettings *sweep_settings;
    // Demod related

    // Preferences/General Settings
    ColorPrefs colors;
    Preferences prefs;

    bool isInPlaybackMode;
};

#endif // SESSION_H
