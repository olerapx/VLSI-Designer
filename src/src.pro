#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T17:55:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VLSI
TEMPLATE = app

SOURCES += main.cpp\
    gui/mainwindow.cpp \
    network/networkscanner.cpp \
    network/networkexception.cpp \
    network/networktransmitter.cpp \
    network/tcpsocket.cpp \
    datamodels/serializable.cpp \
    datamodels/library/pin.cpp \
    datamodels/library/element.cpp \
    datamodels/library/library.cpp \
    datamodels/scheme/schemeelement.cpp \
    datamodels/scheme/wire.cpp \
    datamodels/scheme/scheme.cpp \
    datamodels/grid/grid.cpp \
    datamodels/grid/cell.cpp \
    datamodels/architecture/architecture.cpp \
    exception/exception.cpp \
    exception/illegalargumentexception.cpp \
    util/jsonserializer.cpp

HEADERS  += \
    gui/mainwindow.h \
    network/networkscanner.h \
    network/networkexception.h \
    network/networktransmitter.h \
    network/tcpsocket.h \
    datamodels/serializable.h \
    datamodels/library/pin.h \
    datamodels/library/element.h \
    datamodels/library/library.h \
    datamodels/scheme/schemeelement.h \
    datamodels/scheme/wire.h \
    datamodels/scheme/scheme.h \
    datamodels/grid/grid.h \
    datamodels/grid/cell.h \
    datamodels/architecture/architecture.h \
    exception/exception.h \
    exception/illegalargumentexception.h \
    util/jsonserializer.h

FORMS    += gui/mainwindow.ui
