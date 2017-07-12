#ifndef PRIMARYPLACEMENTALGORITHM_H
#define PRIMARYPLACEMENTALGORITHM_H

#include <QObject>
#include <QList>

#include "algorithms/placement/placementresult.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/scheme.h"

/**
 * @brief The PrimaryPlacementAlgorithm class
 * Base class for primary placement algorithms.
 */
class PrimaryPlacementAlgorithm: public QObject
{
    Q_OBJECT

public:
    PrimaryPlacementAlgorithm(Scheme* scheme, double expandCoefficient);

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

    /**
     * @brief sendFinish
     * Emits when the process is finished or interrupted.
     */
    void sendFinish();

    /**
     * @brief sendError
     * Emits when an exception is occurred.
     * @param error - the exception text.
     */
    void sendError(QString error);

    /**
     * @brief sendLog
     * @param log
     */
    void sendLog(QString log);

public slots:
    /**
     * @brief onStart
     * Starts the algorithm. An alternate way is to call execute().
     */
    void onStart();

    /**
     * @brief onStop
     * Requests the algorithm to stop.
     * When the algorithm will be stopped, a sendFinish will be emitted.
     */
    void onStop();

protected:
    Scheme* scheme;
    double expandCoefficient;

    bool stopped;
    bool actuallyStopped;
};

#endif // PRIMARYPLACEMENTALGORITHM_H
