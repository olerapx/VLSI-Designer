#ifndef PRIMARYPLACEMENTALGORITHM_H
#define PRIMARYPLACEMENTALGORITHM_H

#include <QObject>
#include <QList>

#include "threadable.h"
#include "algorithms/placement/placementresult.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"

/**
 * @brief The PrimaryPlacementAlgorithm class
 * Base class for primary placement algorithms.
 */
class PrimaryPlacementAlgorithm: public Threadable
{
    Q_OBJECT

public:
    PrimaryPlacementAlgorithm(Scheme* scheme, double expandCoefficient);

    void setParameters(Scheme* scheme, double expandCoefficient);

    /**
     * @brief execute
     * Performs the placement of the given elements.
     * @return result.
     */
    virtual PlacementResult* execute() = 0;

signals:

    /**
     * @brief sendResult
     * Emits when the process is finished.
     * @param result - the algorithm result.
     */
    void sendResult(PlacementResult* result);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

protected:
    Scheme* scheme;
    double expandCoefficient;
};

#endif // PRIMARYPLACEMENTALGORITHM_H
