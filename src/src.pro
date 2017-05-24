#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T17:55:40
#
#-------------------------------------------------

TARGET = vlsi
TEMPLATE = lib

INCLUDEPATH += $$PWD
include (src.pri)

SOURCES += \
    network/networkscanner.cpp \
    network/networkexception.cpp \
    network/networktransmitter.cpp \
    network/tcpsocket.cpp \
    datamodels/serializable.cpp \
    datamodels/library/pin.cpp \
    datamodels/library/library.cpp \
    datamodels/scheme/schemeelement.cpp \
    datamodels/scheme/wire.cpp \
    datamodels/scheme/scheme.cpp \
    datamodels/grid/grid.cpp \
    datamodels/grid/cell.cpp \
    datamodels/grid/routedwireindex.cpp \
    datamodels/architecture/architecture.cpp \
    datamodels/architecture/clientsnumber.cpp \
    exception/exception.cpp \
    exception/illegalargumentexception.cpp \
    util/serializer/serializer.cpp \
    util/serializer/jsonserializer.cpp \
    util/serializer/binaryserializer.cpp \
    datamodels/library/libraryelement.cpp \
    generator/generatorparameters.cpp \
    generator/generator.cpp \
    generator/nodeelement.cpp \
    util/renderer/gridrenderer.cpp
