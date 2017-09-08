#pragma once

#include <cmath>
#include <QSize>

#include "algorithms/composition/compositionalgorithm.h"
#include "util/misc/gridutils.h"

/**
 * @brief The PermutationComposition class
 * Performs the composition of the given grid parts into one grid.
 * The passed grid parts will be changed.
 */
class PermutationComposition : public CompositionAlgorithm
{
public:
    PermutationComposition(QList<Grid*> grids);

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

    int gridHeight;
    int gridWidth;

    void clear();

    void stretchParts();
    void getMaxGridSize();
    void stretchHeight(Grid* grid, int number);
    void stretchWidth(Grid* grid, int number);

    void fillPositions();
    void getGridWidth();
    void centerLastRow();

    Grid* buildResult();
    Grid* createEmptyTotalGrid();
};
