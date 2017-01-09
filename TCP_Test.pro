#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T17:55:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCP_Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    network/networkscanner.cpp \
    network/networkexception.cpp \
    network/networktransmitter.cpp

HEADERS  += mainwindow.h \
    network/networkscanner.h \
    network/networkexception.h \
    network/networktransmitter.h

FORMS    += mainwindow.ui
