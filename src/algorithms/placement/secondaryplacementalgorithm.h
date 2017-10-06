#pragma once

#include <QTime>

#include "threadable.h"
#include "algorithms/placement/placementresult.h"
#include "wirecoordinate.h"

/**
 * @brief The SecondaryPlacementAlgorithm class
 * Base class for secondary placement algorithms.
 */
class SecondaryPlacementAlgorithm : public Threadable
{
    Q_OBJECT

public:
    SecondaryPlacementAlgorithm(PlacementResult* previous);
    virtual ~SecondaryPlacementAlgorithm() {}

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
    PlacementResult* previous;

    /**
     * @brief fillWireData
     * Fills in wires data list in the grid. Must be called at the end of any secondary placement algorithm.
     */
    void fillWiresData(PlacementResult* result, QList<WireCoordinate> coordinates);

    QList<WireCoordinate> fillWireCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates);
};
