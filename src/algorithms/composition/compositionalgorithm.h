#pragma once

#include "threadable.h"
#include "datamodels/grid/grid.h"
#include "util/misc/schemeutils.h"

/**
 * @brief The CompositionAlgorithm class
 * Base class for composition algorithms.
 */
class CompositionAlgorithm : public Threadable
{
    Q_OBJECT

public:
    CompositionAlgorithm(QList<Grid*> grids, Scheme* scheme);
    virtual ~CompositionAlgorithm() {}

    /**
     * @brief setParameters
     * Sets new parameters to the algorithm.
     * @param grids
     */
    void setParameters(QList<Grid*> grids, Scheme* scheme);

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
    struct GridPartWireData
    {
        qint64 index;

        QPoint srcCoord;
        int srcGridIndex;

        QPoint destCoord;
        int destGridIndex;

        WireType type;
    };

    struct ExternalWireData
    {
        qint64 index;

        QPoint srcCoord;
        int gridIndex;
    };

    static const int innerWireFitnessCoefficient = 10;

    QList<Grid*> grids;
    Scheme* scheme;

    QList<ExternalWireData> composedExternalWireData;
    QList<GridPartWireData> composedInternalWireData;

    /**
     * @brief fillPartsWireData
     * Retrieves all external wires' data from all grid parts and combines them, filling the partsWireData list.
     * Should be called if composition algorithm uses the wire coordinates to calculate fitness value.
     */
    void fillComposedWireData();

private:
    QList<ExternalWireData> fillExternalWiresData();
};
