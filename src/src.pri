CONFIG -= debug_and_release debug_and_release_target
QT       += core network

INCLUDEPATH += $$PWD

HEADERS  += \
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
