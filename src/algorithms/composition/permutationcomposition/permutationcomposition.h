#pragma once

#include <cmath>
#include <QSize>

#include "algorithms/composition/compositionalgorithm.h"
#include "util/misc/gridutils.h"
#include "util/misc/wireutils.h"

/**
 * @brief The PermutationComposition class
 * Performs the composition of the given grid parts into one grid.
 * The passed grid parts will be changed.
 */
class PermutationComposition : public CompositionAlgorithm
{
    Q_OBJECT

public:
    PermutationComposition(QList<Grid*> grids, Scheme* scheme);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @throw Exception
     * @return composed grid.
     */
    Grid* execute();

private:
    struct GridPosition
    {
        QPoint coord;
        int gridIndex;
    };    

    QSize maxSize;
    QList<GridPosition> positions;
    QList<QPoint> offsets;

    int gridHeight;
    int gridWidth;

    void clear();

    void fillOffsets();
    void getMaxGridSize();

    void fillPositions();
    void getGridWidth();
    void centerLastRow();

    void optimize();
    int getPositionIndexByGridIndex(int gridIndex);
    qint64 getFitnessValueWithPermutation(int firstIndex, int secondIndex);
    qint64 getFitnessValue(QList<GridPosition>& positions);
    int findOptimalPositionIndex(QList<qint64>& fitnessValues);

    Grid* buildResult();
    Grid* createEmptyTotalGrid();
    void fillTotalGrid(Grid* result);
    void fillWiresData(Grid* result);
    QPoint getActualCoord(int gridIndex, QPoint coord);
    QPoint getActualCoord(int gridIndex, QPoint coord, QList<GridPosition>& positions);
    void fillRoutedWires(Grid* result);
};
