#include "grid.h"

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

bool Grid::operator ==(const Grid& other) const
{
    if(cells == other.cells && routedWires == other.routedWires && wiresData == other.wiresData)
        return true;

    return false;
}
