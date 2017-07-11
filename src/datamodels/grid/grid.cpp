#include "grid.h"

bool Grid::operator ==(const Grid& other)
{
    if(cells == other.cells && routedWires == other.routedWires)
        return true;

    return false;
}
