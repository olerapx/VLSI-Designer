#pragma once

#include <QMap>

#include "algorithms/placement/secondaryplacementalgorithm.h"
#include "util/misc/gridutils.h"

/**
 * @brief The RowPermutationPlacement class
 * Performs the row permutation and the element permutation within each row.
 * All elements must be grouped by their height and placed in rows.
 */
class RowPermutationPlacement : public SecondaryPlacementAlgorithm
{
    Q_OBJECT

public:
    RowPermutationPlacement(PlacementResult* previous);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @return
     */
    PlacementResult* execute() override;

    /**
     * @brief getFitnessValue
     * Retrieves the fitness value of the current elements configuration.
     * @return
     */
    qint64 getFitnessValue() override;

private:
    QList<QList<ElementCoordinate>> elementCoordinates;
    QList<WireCoordinate> wireCoordinates;

    QMap<int, int> positions;

    void clear();

    void permutateRows();
    void findOptimalRowPosition(int rowIndex);
    qint64 findFitnessDiffOnRowsSwapping(int firstRowPosition, int secondRowPosition);
    void swapRowsCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates, int firstRowPosition, int secondRowPosition);
    int getShift(QList<ElementCoordinate>& first, QList<ElementCoordinate>& second);

    void permutateRow(int rowIndex);
    void findOptimalElementPosition(int rowIndex, int elementIndex);
    qint64 findFitnessDiffOnElementsSwapping(int rowIndex, int firstElementPosition, int secondElementPosition);    
    void swapElementsCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition);
    int getShift(ElementCoordinate first, ElementCoordinate second);

    qint64 getFitnessValue(QList<WireCoordinate>& wireCoordinates);

    PlacementResult* buildResult();
    void rebuildGrid();
    QList<ElementCoordinate> concatElementCoordinates();
};
