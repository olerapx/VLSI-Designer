#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T17:55:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VLSI
TEMPLATE = app


SOURCES += src/main.cpp\
        src/gui/mainwindow.cpp \
    src/network/networkscanner.cpp \
    src/network/networkexception.cpp \
    src/network/networktransmitter.cpp \
    src/network/tcpsocket.cpp \
    src/datamodels/serializable.cpp \
    src/datamodels/pin.cpp \
    src/exception/exception.cpp \
    src/exception/illegalargumentexception.cpp \
    src/datamodels/element.cpp

HEADERS  += src/gui/mainwindow.h \
    src/network/networkscanner.h \
    src/network/networkexception.h \
    src/network/networktransmitter.h \
    src/network/tcpsocket.h \
    src/datamodels/serializable.h \
    src/datamodels/pin.h \
    src/exception/exception.h \
    src/exception/illegalargumentexception.h \
    src/datamodels/element.h

FORMS    += src/gui/mainwindow.ui
