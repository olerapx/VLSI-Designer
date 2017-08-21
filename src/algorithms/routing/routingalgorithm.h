#ifndef ROUTINGALGORITHM_H
#define ROUTINGALGORITHM_H

#include "threadable.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"
#include "routingexception.h"

enum class Direction
{
    Left,
    Up,
    Right,
    Down
};

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
    RoutingAlgorithm(Grid* grid, Scheme* scheme);

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param grid
     * @param scheme
     */
    void setParameters(Grid* grid, Scheme* scheme);

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

    bool canEnter(int x, int y, Direction direction);
    bool hasElementNearby(int x, int y);

    bool canLeave(int x, int y, Direction direction);

    void draw(Cell& cell, Direction from, Direction to);
    CellType getDrawType(CellType type, Direction from, Direction to);

    void branch(Cell& cell, Direction to);
    CellType getBranchType(CellType type, Direction to);

    RoutingState canRoute(QPoint from, QPoint to);
};

#endif // ROUTINGALGORITHM_H
