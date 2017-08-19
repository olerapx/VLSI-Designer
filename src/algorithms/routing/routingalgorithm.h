#ifndef ROUTINGALGORITHM_H
#define ROUTINGALGORITHM_H

#include "threadable.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"

enum class Direction
{
    Left,
    Right,
    Up,
    Down
};

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
};

#endif // ROUTINGALGORITHM_H
