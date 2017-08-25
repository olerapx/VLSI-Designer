#ifndef ROUTINGALGORITHM_H
#define ROUTINGALGORITHM_H

#include "threadable.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"
#include "routingexception.h"
#include "direction.h"
#include "algorithms/placement/primaryplacementalgorithm.h"

enum RoutingAction
{
    Branch,
    Draw,
    Nothing,
    WarnBrokenWire
};

struct RoutingState
{
    bool canMove;
    RoutingAction action;
    bool newBranched;
};

Direction operator !(const Direction& other);

/**
 * @brief The RoutingAlgorithm class
 * Base class for routing algorithms.
 */
class RoutingAlgorithm : public Threadable
{
    Q_OBJECT

public:
    RoutingAlgorithm(Grid* grid, Scheme* scheme, PrimaryPlacementAlgorithm* algorithm, int maxExtensionAttempts);

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param grid
     * @param scheme
     * @param algorithm
     * @param maxExtensionAttempts
     */
    void setParameters(Grid* grid, Scheme* scheme, PrimaryPlacementAlgorithm* algorithm, int maxExtensionAttempts);

    /**
     * @brief execute
     * Performs the routing on the given grid.
     * @return result.
     */
    virtual Grid* execute() = 0;

signals:
    /**
     * @brief sendResult
     * Emits when the process is finished.
     * @param grid - the changed grid. This is not a copy of the initial grid.
     */
    void sendResult(Grid* grid);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

protected:
    Grid* grid;
    Scheme* scheme;
    PrimaryPlacementAlgorithm* primaryPlacement;
    int maxExtensionAttempts;

    bool canEnter(QPoint coord, Direction from);
    bool validateCoord(QPoint coord);
    bool hasElementNearby(QPoint coord);

    bool canLeave(QPoint coord, Direction to);

    void draw(Cell& cell, Direction from, Direction to);
    void branch(Cell& cell, Direction to);

    RoutingState canRoute(QPoint from, QPoint to, bool branched);
    Direction getDirection(QPoint from, QPoint to);

    bool extend(QPoint coord, int number, Direction direction);

private:
    CellType getDrawType(CellType type, Direction from, Direction to);
    CellType getBranchType(CellType type, Direction to);

    bool extendHorizontally(QPoint coord, int number, Direction direction);
    bool extendVertically(QPoint coord, int number, Direction direction);
};

#endif // ROUTINGALGORITHM_H
