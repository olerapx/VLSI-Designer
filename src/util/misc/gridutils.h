#pragma once

#include "datamodels/grid/grid.h"

class GridUtils
{
public:
    static QList<QList<Cell>> cut(Grid* grid, QPoint topLeftCoord, int width, int height);
    static void paste(Grid* grid, QList<QList<Cell>> data, QPoint topLeftCoord);

    static void insertRows(Grid* grid, int y, int number);
    static void removeRows(Grid* grid, int y, int number);

    static void insertEmptyArea(Grid* grid, QPoint topLeftCoord, int colsNumber, int rowsNumber);
    static void removeArea(Grid* grid, QPoint topLeftCoord, int colsNumber, int rowsNumber);
};
