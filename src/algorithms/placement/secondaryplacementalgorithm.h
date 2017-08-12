#ifndef SECONDARYPLACEMENTALGORITHM_H
#define SECONDARYPLACEMENTALGORITHM_H

#include "threadable.h"
#include "algorithms/placement/placementresult.h"

/**
 * @brief The SecondaryPlacementAlgorithm class
 * Base class for secondary placement algorithms.
 */
class SecondaryPlacementAlgorithm : public Threadable
{
    Q_OBJECT

public:
    SecondaryPlacementAlgorithm(PlacementResult* previous);

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param previous
     */
    void setParameters(PlacementResult* previous);

    /**
     * @brief execute
     * Performs the secondary placement of the given elements.
     * @return
     */
    virtual PlacementResult* execute() = 0;

    /**
     * @brief getFitnessValue
     * Retrieves the fitness value of the current elements configuration.
     * @return
     */
    virtual qint64 getFitnessValue() = 0;

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
    PlacementResult* previous;
};

#endif // SECONDARYPLACEMENTALGORITHM_H
