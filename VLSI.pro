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
    src/datamodels/library/pin.cpp \
    src/datamodels/library/element.cpp \
    src/datamodels/library/library.cpp \
    src/datamodels/scheme/schemeelement.cpp \
    src/datamodels/scheme/wire.cpp \
    src/datamodels/scheme/scheme.cpp \
    src/datamodels/grid/grid.cpp \
    src/datamodels/grid/cell.cpp \
    src/datamodels/architecture/architecture.cpp \
    src/exception/exception.cpp \
    src/exception/illegalargumentexception.cpp \
    src/util/jsonserializer.cpp

HEADERS  += \
    src/gui/mainwindow.h \
    src/network/networkscanner.h \
    src/network/networkexception.h \
    src/network/networktransmitter.h \
    src/network/tcpsocket.h \
    src/datamodels/serializable.h \
    src/datamodels/library/pin.h \
    src/datamodels/library/element.h \
    src/datamodels/library/library.h \
    src/datamodels/scheme/schemeelement.h \
    src/datamodels/scheme/wire.h \
    src/datamodels/scheme/scheme.h \
    src/datamodels/grid/grid.h \
    src/datamodels/grid/cell.h \
    src/datamodels/architecture/architecture.h \
    src/exception/exception.h \
    src/exception/illegalargumentexception.h \
    src/util/jsonserializer.h

FORMS    += src/gui/mainwindow.ui
