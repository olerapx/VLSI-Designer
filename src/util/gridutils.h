#ifndef GRIDUTILS_H
#define GRIDUTILS_H

#include "datamodels/grid/grid.h"

class GridUtils
{
public:
    static QList<QList<Cell>> cut(Grid* grid, QPoint topLeftCoord, int width, int height);
    static void paste(Grid* grid, QList<QList<Cell>> data, QPoint topLeftCoord);

    static void insertEmptyArea(Grid* grid, QPoint topLeftCoord, int width, int height);
    static void removeArea(Grid* grid, QPoint topLeftCoord, int width, int height);
};

#endif // GRIDUTILS_H
