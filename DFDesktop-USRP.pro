#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T19:18:17
#
#-------------------------------------------------

QT       += core gui network charts opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DF-Desktop
TEMPLATE = app
CONFIG += c++11
DESTDIR += $$PWD/bin
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS -= -Zc:strictStrings
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        src/mainwindow.cpp \
    src/globals.cpp \
    src/widgets/dock_page.cpp \
    src/widgets/dock_panel.cpp \
    src/widgets/entry_widgets.cpp \
    src/widgets/status_bar.cpp \
    src/lib/amplitude.cpp \
    src/lib/bb_lib.cpp \
    src/lib/device_traits.cpp \
    src/lib/frequency.cpp \
    src/lib/time_type.cpp \
    src/desk_panel.cpp \
    src/device.cpp \
    src/df_settings.cpp \
    src/factory.cpp \
    src/session.cpp \
    src/sweep_central.cpp \
    src/newfiledialog.cpp \
    src/samples_importing_dialog.cpp \
    src/testdialog.cpp \
    src/detection_dlg.cpp \
    src/datasource.cpp \
    src/glchartview.cpp \
    src/simpletable.cpp \
    src/tablesource.cpp \
    src/waterfall_plot.cpp \
    src/widgets/signalgen_page.cpp \
    src/signal_generator.cpp \
    src/data_collecting_dlg.cpp \
    src/field_test_dlg.cpp \
    src/browse_dlg.cpp \
    src/usrp_x310_2ch.cpp

HEADERS  += src/mainwindow.h \
    src/globals.h \
    src/grid_mod.h \
    src/widgets/dock_page.h \
    src/widgets/dock_panel.h \
    src/widgets/entry_widgets.h \
    src/widgets/status_bar.h \
    src/lib/amplitude.h \
    src/lib/bb_lib.h \
    src/lib/device_traits.h \
    src/lib/frequency.h \
    src/lib/macros.h \
    src/lib/sa_api.h \
    src/lib/threadsafe_queue.h \
    src/lib/time_type.h \
    src/widgets/progress_dialog.h \
    src/desk_panel.h \
    src/device.h \
    src/df_lib.h \
    src/df_settings.h \
    src/factory.h \
    src/session.h \
    src/sweep_central.h \
    src/newfiledialog.h \
    src/samples_importing_dialog.h \
    src/testdialog.h \
    src/detection_dlg.h \
    src/datasource.h \
    src/glchartview.h \
    src/simpletable.h \
    src/tablesource.h \
    src/waterfall_plot.h \
    src/widgets/signalgen_page.h \
    src/signal_generator.h \
    src/data_collecting_dlg.h \
    src/field_test_dlg.h \
    src/browse_dlg.h \
    include/libci.h \
    src/usrp_x310_2ch.h

INCLUDEPATH += $$PWD/src

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


RESOURCES     = application.qrc

TRANSLATIONS += desktop_cn.ts


unix:!macx: LIBS += -lboost_program_options -luhd -lboost_system
