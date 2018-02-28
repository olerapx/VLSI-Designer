#pragma once

#include "datamodels/grid/grid.h"
#include "datamodels/scheme/schemeelement.h"
#include "libraryutils.h"

class GridUtils
{
public:
    static void removeRows(Grid* grid, int y, int number);
    static void removeArea(Grid* grid, QPoint topLeftCoord, int colsNumber, int rowsNumber);

    static Grid* rotate90(Grid* grid);

    static void drawElement(Grid* grid, QPoint point, SchemeElement element, QList<Library*> libraries);

private:
    static CellType rotateCellType(CellType type);
};
