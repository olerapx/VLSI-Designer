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
    threadable.cpp \
    algorithms/decomposition/decompositionalgorithm.cpp \
    algorithms/decomposition/serialdecomposition/serialdecomposition.cpp \
    algorithms/decomposition/serialdecomposition/schemevertex.cpp \
    algorithms/decomposition/serialdecomposition/outerconnectionsnumbercomparator.cpp \
    algorithms/placement/primaryplacementalgorithm.cpp \
    algorithms/placement/secondaryplacementalgorithm.cpp \
    algorithms/placement/placementresult.cpp \
    algorithms/placement/elementcoordinate.cpp \
    algorithms/placement/wirecoordinate.cpp \
    algorithms/placement/rowwiseplacement/rowwiseplacement.cpp \
    algorithms/placement/rowwiseplacement/elementheightcomparator.cpp \
    algorithms/placement/rowpermutationplacement/rowpermutationplacement.cpp \
    algorithms/routing/routingalgorithm.cpp \
    algorithms/routing/leerouting/leerouting.cpp \
    network/networkscanner.cpp \
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
    datamodels/grid/wiredata.cpp \
    datamodels/grid/routedwireindex.cpp \
    datamodels/architecture/architecture.cpp \
    datamodels/architecture/clientsnumber.cpp \
    exception/exception.cpp \
    util/serializer/serializer.cpp \
    util/serializer/jsonserializer.cpp \
    util/serializer/binaryserializer.cpp \
    util/misc/libraryutils.cpp \
    util/misc/gridutils.cpp \
    util/misc/wireutils.cpp \
    datamodels/library/libraryelement.cpp \
    generator/generatorparameters.cpp \
    generator/generator.cpp \
    generator/nodeelement.cpp \
    util/renderer/gridrenderer.cpp \
    util/aliaser/aliaser.cpp

TRANSLATIONS = \
    ../resources/localization/src_en_US.ts  \
    ../resources/localization/src_ru_RU.ts
