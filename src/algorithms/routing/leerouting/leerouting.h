#pragma once

#include "algorithms/routing/routingalgorithm.h"
#include "util/misc/schemeutils.h"
#include "util/misc/wireutils.h"
#include "wiredistancecomparator.h"
#include "pointdistancecomparator.h"

/**
 * @brief The LeeRouting class
 * Implements the Lee path connection algorithm.
 */
class LeeRouting : public RoutingAlgorithm
{
    Q_OBJECT

public:
    LeeRouting(Grid* grid, Scheme* scheme, int maxExtensionAttempts, int maxExtensionAttemptsForWire);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @throw Exception
     * @return
     */
    Grid* execute() override;

private:
    struct CellInfo
    {
        qint64 value;
        bool branched;
    };

    QList<WireData*> inNodeWires, interNodeWires;
    QList<QList<CellInfo>> matrix;

    qint64 currentValue;
    int extensionAttemptsForWire;

    bool finished;
    bool noMoreWays;

    QPoint startCoord, finishCoord;
    QPoint startPinCoord, finishPinCoord;
    bool startBranched;

    QList<ExtensionRecord> extensions;
    QList<QPoint> currentWavePoints;
    QList<QPoint> nextWavePoints;

    void clear();

    void initWires();
    bool isWireRouted(Wire& wire);

    void routeWire(WireData *data);    
    void initMatrix();
    void getTerminalCoords(WireData* data);
    std::pair<QPoint, bool> getNearbyAvailableCoord(QPoint pinCoord);

    void pushWave();
    bool tryRoute(QPoint from, QPoint to);

    bool tryExtend();
    void undoAllExtends();

    void pushReverseWave();
    bool canAdvance(QPoint currentCoord, QPoint coord);
};
