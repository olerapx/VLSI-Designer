#ifndef ROWPERMUTATIONPLACEMENT_H
#define ROWPERMUTATIONPLACEMENT_H

#include <QMap>

#include "algorithms/placement/secondaryplacementalgorithm.h"
#include "algorithms/placement/wirecoordinate.h"
#include "util/gridutils.h"

/**
 * @brief The RowPermutationPlacement class
 * Performs the row permutation and the element permutation within each row.
 * All elements must be grouped by their height and placed in rows.
 */
class RowPermutationPlacement: public SecondaryPlacementAlgorithm
{
public:
    RowPermutationPlacement(PlacementResult* previous);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @return
     */
    PlacementResult* execute();

    qint64 getFitnessValue();

private:
    QList<QList<ElementCoordinate>> elementCoordinates;
    QList<WireCoordinate> wireCoordinates;

    QMap<int, int> positions;

    void clear();
    QList<WireCoordinate> fillWireCoordinates(QList<ElementCoordinate> &elementCoordinates);

    void permutateRows();

    void permutateRow(int rowIndex);
    void findOptimalElementPosition(int rowIndex, int elementIndex);
    qint64 findFitnessDiffOnElementsSwapping(int rowIndex, int firstElementPosition, int secondElementPosition);

    qint64 getFitnessValue(QList<WireCoordinate>& wireCoordinates);

    void swapElements(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition);
    int getRowHeight(QList<ElementCoordinate>& row);

    PlacementResult* buildResult();
    QList<ElementCoordinate> concatCoordinates();
};

#endif // ROWPERMUTATIONPLACEMENT_H
