#include "grid.h"

Grid::Grid(int initialLevel)
{
    setInitialLevel(initialLevel);
}

void Grid::setInitialLevel(int initialLevel)
{
    if (initialLevel < 0)
        throw IllegalArgumentException("Client initial level cannot be negative");

    this->initialLevel = initialLevel;
}

bool Grid::operator ==(const Grid& other)
{
    if(cells == other.cells && routedWires == other.routedWires &&
            initialLevel == other.initialLevel)
        return true;

    return false;
}
