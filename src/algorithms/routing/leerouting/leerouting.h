#ifndef LEEROUTING_H
#define LEEROUTING_H

#include <QList>

#include "algorithms/routing/routingalgorithm.h"
#include "util/misc/schemeutils.h"
#include "util/misc/wireutils.h"
#include "wiredistancecomparator.h"

struct CellInfo
{
    int value;
    bool branched;
};

/**
 * @brief The LeeRouting class
 * Implements the Lee path connection algorithm.
 */
class LeeRouting : public RoutingAlgorithm
{
public:
    LeeRouting(Grid* grid, Scheme* scheme, PrimaryPlacementAlgorithm* algorithm, int maxExtensionAttempts);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @throw Exception
     * @return
     */
    Grid* execute();

private:
    QList<WireData> innerWires, outerWires;
    QList<QList<CellInfo>> matrix;

    int gridHeight, gridWidth;

    void clear();

    void initMatrix();
    void initWires();
    bool isWireRouted(Wire& wire);

    void routeWire(WireData data);
    RoutingState canRoute(QPoint from, QPoint to);
};

#endif // LEEROUTING_H
