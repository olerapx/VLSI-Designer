CONFIG -= debug_and_release debug_and_release_target
QT += core network

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/threadable.h \
    $$PWD/algorithms/composition/compositionalgorithm.h \
    $$PWD/algorithms/composition/permutationcomposition/permutationcomposition.h \
    $$PWD/algorithms/decomposition/decompositionalgorithm.h \
    $$PWD/algorithms/decomposition/serialdecomposition/serialdecomposition.h \
    $$PWD/algorithms/decomposition/serialdecomposition/schemevertex.h \
    $$PWD/algorithms/decomposition/serialdecomposition/outerconnectionsnumbercomparator.h \
    $$PWD/algorithms/placement/primaryplacementalgorithm.h \
    $$PWD/algorithms/placement/secondaryplacementalgorithm.h \
    $$PWD/algorithms/placement/placementresult.h \
    $$PWD/algorithms/placement/elementcoordinate.h \
    $$PWD/algorithms/placement/wirecoordinate.h \
    $$PWD/algorithms/placement/rowwiseplacement/rowwiseplacement.h \
    $$PWD/algorithms/placement/rowwiseplacement/elementheightcomparator.h \
    $$PWD/algorithms/placement/rowpermutationplacement/rowpermutationplacement.h \
    $$PWD/algorithms/routing/routingalgorithm.h \
    $$PWD/algorithms/routing/routingexception.h \
    $$PWD/algorithms/routing/leerouting/leerouting.h \
    $$PWD/algorithms/routing/leerouting/wiredistancecomparator.h \
    $$PWD/algorithms/routing/leerouting/pointdistancecomparator.h \
    $$PWD/control/pool/entitytype.h \
    $$PWD/control/pool/poolentity.h \
    $$PWD/control/pool/poolentityinfo.h \
    $$PWD/control/command/command.h \
    $$PWD/control/command/commanddispatcher.h \
    $$PWD/control/pool/manager/commandhistoryentry.h \
    $$PWD/control/pool/manager/poolmanager.h \
    $$PWD/control/pool/manager/sessiondata.h \
    $$PWD/control/pool/node/poolnode.h \
    $$PWD/network/networkscanner.h \
    $$PWD/network/networkexception.h \
    $$PWD/network/networktransmitter.h \
    $$PWD/network/tcpsocket.h \
    $$PWD/datamodels/serializable.h \
    $$PWD/datamodels/library/pin.h \
    $$PWD/datamodels/library/library.h \
    $$PWD/datamodels/scheme/schemeelement.h \
    $$PWD/datamodels/scheme/wire.h \
    $$PWD/datamodels/scheme/scheme.h \
    $$PWD/datamodels/grid/grid.h \
    $$PWD/datamodels/grid/cell.h \
    $$PWD/datamodels/grid/wiredata.h \
    $$PWD/datamodels/architecture/architecture.h \
    $$PWD/datamodels/architecture/algorithmindexes.h \
    $$PWD/datamodels/architecture/algorithmparameters.h \
    $$PWD/datamodels/version/version.h \
    $$PWD/exception/exception.h \
    $$PWD/exception/illegalargumentexception.h \
    $$PWD/exception/threadstoppedexception.h \
    $$PWD/exception/ioexception.h \
    $$PWD/util/serializer/serializer.h \
    $$PWD/util/serializer/serializerstrategy.h \
    $$PWD/util/serializer/jsonserializer.h \
    $$PWD/util/serializer/binaryserializer.h \
    $$PWD/util/misc/libraryutils.h \
    $$PWD/util/misc/gridutils.h \
    $$PWD/util/misc/wireutils.h \
    $$PWD/util/misc/schemeutils.h \
    $$PWD/datamodels/grid/routedwireindex.h \
    $$PWD/datamodels/architecture/clientsnumber.h \
    $$PWD/datamodels/library/libraryelement.h \
    $$PWD/generator/generatorparameters.h \
    $$PWD/generator/generator.h \
    $$PWD/generator/nodeelement.h \
    $$PWD/util/renderer/gridrenderer.h \
    $$PWD/util/aliaser/aliaser.h \
    $$PWD/algorithms/composition/compositionstrategy.h \
    $$PWD/algorithms/decomposition/decompositionstrategy.h \
    $$PWD/algorithms/placement/primaryplacementstrategy.h \
    $$PWD/algorithms/placement/secondaryplacementstrategy.h \
    $$PWD/algorithms/routing/routingstrategy.h

RESOURCES += \
    $$PWD/../resources/resources.qrc
