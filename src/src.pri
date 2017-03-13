CONFIG -= debug_and_release debug_and_release_target

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/gui/mainwindow.cpp \
    $$PWD/network/networkscanner.cpp \
    $$PWD/network/networkexception.cpp \
    $$PWD/network/networktransmitter.cpp \
    $$PWD/network/tcpsocket.cpp \
    $$PWD/datamodels/serializable.cpp \
    $$PWD/datamodels/library/pin.cpp \
    $$PWD/datamodels/library/element.cpp \
    $$PWD/datamodels/library/library.cpp \
    $$PWD/datamodels/scheme/schemeelement.cpp \
    $$PWD/datamodels/scheme/wire.cpp \
    $$PWD/datamodels/scheme/scheme.cpp \
    $$PWD/datamodels/grid/grid.cpp \
    $$PWD/datamodels/grid/cell.cpp \
    $$PWD/datamodels/architecture/architecture.cpp \
    $$PWD/exception/exception.cpp \
    $$PWD/exception/illegalargumentexception.cpp \
    $$PWD/util/jsonserializer.cpp

HEADERS  += \
    $$PWD/gui/mainwindow.h \
    $$PWD/network/networkscanner.h \
    $$PWD/network/networkexception.h \
    $$PWD/network/networktransmitter.h \
    $$PWD/network/tcpsocket.h \
    $$PWD/datamodels/serializable.h \
    $$PWD/datamodels/library/pin.h \
    $$PWD/datamodels/library/element.h \
    $$PWD/datamodels/library/library.h \
    $$PWD/datamodels/scheme/schemeelement.h \
    $$PWD/datamodels/scheme/wire.h \
    $$PWD/datamodels/scheme/scheme.h \
    $$PWD/datamodels/grid/grid.h \
    $$PWD/datamodels/grid/cell.h \
    $$PWD/datamodels/architecture/architecture.h \
    $$PWD/exception/exception.h \
    $$PWD/exception/illegalargumentexception.h \
    $$PWD/util/jsonserializer.h

FORMS    += $$PWD/gui/mainwindow.ui
