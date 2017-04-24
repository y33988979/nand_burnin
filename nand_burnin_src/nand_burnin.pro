#-------------------------------------------------
#
# Project created by QtCreator 2016-09-02T11:17:51
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Nand_burnin
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mainwindow.cpp \
    HwInfoDialog.cpp \
    SerialControl.cpp \
    NandBanner.cpp \
    TestParamDialog.cpp \
    SerialDebugWidget.cpp \
    SerialConf.cpp

HEADERS  += widget.h \
    mainwindow.h \
    HwInfoDialog.h \
    common.h \
    SerialControl.h \
    NandBanner.h \
    TestParamDialog.h \
    SerialDebugWidget.h \
    SerialConf.h \
    globalvariables.h

FORMS    += widget.ui \
    mainwindow.ui \
    serialconf.ui \
    HwInfoDialog.ui \
    TestParamDialog.ui
