#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T02:11:29
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeedGauge
TEMPLATE = app


SOURCES += main.cpp\
    ../../source/mytcp.cpp \
    ../../source/qcthemeitem.cpp \
    ../../source/waterLoopGaugeItem.cpp \
        mainwindow.cpp \
    ../../source/qcgaugewidget.cpp

HEADERS  += mainwindow.h \
    ../../source/mytcp.h \
    ../../source/qcgaugewidget.h \
    ../../source/qcthemeitem.h \
    ../../source/waterLoopGaugeItem.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources/resources.qrc

