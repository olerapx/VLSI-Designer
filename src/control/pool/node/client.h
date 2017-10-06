#pragma once

#include <QThread>

#include "algorithms/decomposition/decompositionstrategy.h"
#include "algorithms/placement/primaryplacementstrategy.h"
#include "algorithms/placement/secondaryplacementstrategy.h"
#include "algorithms/routing/routingstrategy.h"
#include "algorithms/composition/compositionstrategy.h"
#include "datamodels/architecture/architecture.h"
#include "control/pool/logtype.h"
#include "datamodels/statistics/statisticsentry.h"

/**
 * @brief The Client class
 * Encapsulates working with algorithms. Does not deallocate any input data.
 */
class Client : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Client
     * Constructs an empty object.
     */
    Client();
    ~Client();

    void setArchitecture(Architecture* a);
    void setLibraries(QList<Library*> list);

    Architecture* getArchitecture() { return architecture; }
    QList<Library*> getLibraries() { return libraries; }

    bool isStopped() const { return stopped; }
    void stop();

    /**
     * @brief startDecomposition
     * Starts the decomposition process.
     * The result will be sent in the sendDecomposedSchemes signal.
     * @param scheme
     * @param number
     */
    void startDecomposition(Scheme* scheme, int number, StatisticsEntry& entry);

    /**
     * @brief startPlacingAndRouting
     * Starts the process of placing and inner routing.
     * The result will be sent in the sendRoutedGrid signal.
     * @param scheme
     */
    void startPlacingAndRouting(Scheme* scheme, StatisticsEntry& entry);

    /**
     * @brief startComposition
     * Starts the process of composition.
     * The result will be sent in the sendComposedGrid signal.
     * @param grids
     * @param scheme
     * @param level - the level of grid parts in the distribution hierarchy. The composed grid will have level - 1.
     */
    void startComposition(QList<Grid*> grids, Scheme* scheme, int level, StatisticsEntry& entry);

private slots:
    void onLog(QString log);

    void onPrimaryPlacementFinished(PlacementResult* res);
    void onSecondaryPlacementFinished(PlacementResult* res);

    void onCompositionFinished(Grid* grid);    

signals:
    void sendLog(QString log, LogType type = LogType::Common);
    void sendError(QString error);    
    void sendDecomposedSchemes(QList<Scheme*> schemes);
    void sendRoutedGrid(Grid* grid);
    void sendComposedGrid(Grid* grid, int level);

    void sendStop();

private:
    bool isInitialized() const;

    void moveAlgorithmToThread(Threadable* t);

    Scheme* scheme;
    StatisticsEntry* entry;
    Grid* grid;

    Architecture* architecture;
    QList<Library*> libraries;

    QThread algorithmThread;

    DecompositionAlgorithm* decomposition;
    PrimaryPlacementAlgorithm* primaryPlacement;
    SecondaryPlacementAlgorithm* secondaryPlacement;
    RoutingAlgorithm* routing;
    CompositionAlgorithm* composition;

    int level;
    bool stopped;
};
