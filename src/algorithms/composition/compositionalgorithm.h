#pragma once

#include "threadable.h"
#include "datamodels/grid/grid.h"

/**
 * @brief The CompositionAlgorithm class
 * Base class for composition algorithms.
 */
class CompositionAlgorithm : public Threadable
{
    Q_OBJECT

public:
    CompositionAlgorithm(QList<Grid*> grids);

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param grids
     */
    void setParameters(QList<Grid*> grids);

    /**
     * @brief execute
     * Performs the compositon of the given grid parts into one grid.
     * @return the composed grid.
     */
    virtual Grid* execute() = 0;

signals:
    /**
     * @brief sendResult
     * Emits when the process is finished.
     * @param grid - the composed grid.
     */
    void sendResult(Grid* grid);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

protected:
    QList<Grid*> grids;
};
