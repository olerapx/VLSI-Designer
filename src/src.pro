#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T17:55:40
#
#-------------------------------------------------

TARGET = vlsi
TEMPLATE = lib

INCLUDEPATH += $$PWD
include(src.pri)

SOURCES += \
    threadable.cpp \
    algorithms/composition/compositionalgorithm.cpp \
    algorithms/composition/compositionstrategy.cpp \
    algorithms/composition/permutationcomposition/permutationcomposition.cpp \
    algorithms/decomposition/decompositionalgorithm.cpp \
    algorithms/decomposition/decompositionstrategy.cpp \
    algorithms/decomposition/serialdecomposition/serialdecomposition.cpp \
    algorithms/decomposition/serialdecomposition/schemevertex.cpp \
    algorithms/decomposition/serialdecomposition/outerconnectionsnumbercomparator.cpp \
    algorithms/placement/primaryplacementalgorithm.cpp \
    algorithms/placement/secondaryplacementalgorithm.cpp \
    algorithms/placement/primaryplacementstrategy.cpp \
    algorithms/placement/secondaryplacementstrategy.cpp \
    algorithms/placement/placementresult.cpp \
    algorithms/placement/elementcoordinate.cpp \
    algorithms/placement/wirecoordinate.cpp \
    algorithms/placement/rowwiseplacement/rowwiseplacement.cpp \
    algorithms/placement/rowwiseplacement/elementheightcomparator.cpp \
    algorithms/placement/rowpermutationplacement/rowpermutationplacement.cpp \
    algorithms/routing/routingalgorithm.cpp \
    algorithms/routing/leerouting/leerouting.cpp \
    algorithms/routing/leerouting/wiredistancecomparator.cpp \
    algorithms/routing/leerouting/pointdistancecomparator.cpp \
    algorithms/routing/routingstrategy.cpp \
    control/distribution/distributor.cpp \
    control/distribution/defaultdistributor.cpp \
    control/pool/poolentity.cpp \
    control/pool/poolentityinfo.cpp \
    control/command/command.cpp \
    control/command/commanddispatcher.cpp \
    control/pool/manager/commandhistoryentry.cpp \
    control/pool/manager/poolmanager.cpp \
    control/pool/manager/sessiondata.cpp \
    control/pool/node/poolnode.cpp \
    control/pool/node/client.cpp \
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
    datamodels/architecture/algorithmindexes.cpp \
    datamodels/architecture/algorithmparameters.cpp \
    datamodels/version/version.cpp \
    exception/exception.cpp \
    util/serializer/serializerstrategy.cpp \
    util/serializer/jsonserializer.cpp \
    util/serializer/binaryserializer.cpp \
    util/misc/libraryutils.cpp \
    util/misc/gridutils.cpp \
    util/misc/wireutils.cpp \
    util/misc/schemeutils.cpp \
    datamodels/library/libraryelement.cpp \
    generator/generatorparameters.cpp \
    generator/generator.cpp \
    generator/nodeelement.cpp \
    util/renderer/gridrenderer.cpp \
    util/aliaser/aliaser.cpp

TRANSLATIONS = \
    ../resources/localization/src_en_US.ts  \
    ../resources/localization/src_ru_RU.ts

HEADERS +=
