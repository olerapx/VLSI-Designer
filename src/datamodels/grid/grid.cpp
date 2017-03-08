#include "grid.h"

Grid::Grid(int initialLevel)
{
    setInitialLevel(initialLevel);
}

Grid::~Grid()
{
    foreach(QList<Cell*> list, cells)
    {
        foreach (Cell* cell, list)
            delete cell;
        list.clear();
    }
    cells.clear();
}

void Grid::setInitialLevel(int initialLevel)
{
    if (initialLevel < 0)
        throw IllegalArgumentException("Client initial level cannot be negative");
    this->initialLevel = initialLevel;
}
