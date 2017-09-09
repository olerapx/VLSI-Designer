#include "grid.h"

bool Grid::operator ==(const Grid& other)
{
    if(cells == other.cells && routedWires == other.routedWires && wiresData == other.wiresData)
        return true;

    return false;
}

int Grid::getHeight()
{
    return cells.size();
}

int Grid::getWidth()
{
    if(cells.size() == 0)
        return 0;

    return cells[0].size();
}
