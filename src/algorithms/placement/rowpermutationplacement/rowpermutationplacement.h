#ifndef ROWPERMUTATIONPLACEMENT_H
#define ROWPERMUTATIONPLACEMENT_H

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
public:
    RowPermutationPlacement(PlacementResult* previous);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @return
     */
    PlacementResult* execute();

    /**
     * @brief getFitnessValue
     * Retrieves the fitness value of the current elements configuration.
     * @return
     */
    qint64 getFitnessValue();

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
    void swapRowsOnGrid(QList<QList<ElementCoordinate>>& elementCoordinates, int firstRowPosition, int secondRowPosition);

    void permutateRow(int rowIndex);
    void findOptimalElementPosition(int rowIndex, int elementIndex);
    qint64 findFitnessDiffOnElementsSwapping(int rowIndex, int firstElementPosition, int secondElementPosition);    
    void swapElementsCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition);
    int getShift(ElementCoordinate first, ElementCoordinate second);
    void swapElementsOnGrid(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition);
    int getRowHeight(QList<ElementCoordinate>& row);

    qint64 getFitnessValue(QList<WireCoordinate>& wireCoordinates);

    PlacementResult* buildResult();
    QList<ElementCoordinate> concatElementCoordinates();
};

#endif // ROWPERMUTATIONPLACEMENT_H
