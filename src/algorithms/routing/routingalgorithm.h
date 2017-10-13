#pragma once

#include <QTime>

#include "threadable.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"
#include "routingexception.h"
#include "util/misc/gridutils.h"

enum class Direction
{
    Left,
    Up,
    Right,
    Down
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
    RoutingAlgorithm(Grid* grid, Scheme* scheme, int maxExtensionAttempts, int maxExtensionAttemptsForWire);
    virtual ~RoutingAlgorithm() {}

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param grid
     * @param scheme
     * @param algorithm
     * @param maxExtensionAttempts
     */
    void setParameters(Grid* grid, Scheme* scheme, int maxExtensionAttempts, int maxExtensionAttemptsForWire);

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

    /**
     * @brief sendTime
     * Sends the algorithm execution time.
     * @param msec
     */
    void sendTime(int msec);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

protected:
    struct RoutingState
    {
        bool canMove;
        bool newBranched;
    };

    struct ExtensionRecord
    {
        QPoint coord;
        int number;
        Direction direction;
    };

    Grid* grid;
    Scheme* scheme;

    int maxExtensionAttempts;
    int maxExtensionAttemptsForWire;

    bool canEnter(QPoint coord, Direction from);
    bool checkCoordIsInGrid(QPoint coord);
    bool hasElementNearby(QPoint coord);

    bool canLeave(QPoint coord, Direction to);

    bool draw(Cell& cell, Direction from, Direction to);
    bool branch(Cell& cell, Direction to);

    RoutingAlgorithm::RoutingState canRoute(QPoint from, QPoint to, bool branched);
    Direction getDirection(QPoint from, QPoint to);

    bool extend(QPoint coord, int number, Direction direction);
    void undoExtends(QList<ExtensionRecord>& extensions);

private:
    CellType getDrawType(CellType type, Direction from, Direction to);
    CellType getBranchType(CellType type, Direction to);

    bool extendHorizontally(QPoint coord, int number, Direction direction);
    bool extendVertically(QPoint coord, int number, Direction direction);

    void undoHorizontalExtension(ExtensionRecord& record);
    void undoVerticalExtension(ExtensionRecord& record);
};
