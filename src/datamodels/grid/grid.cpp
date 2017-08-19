#include "grid.h"

bool Grid::operator ==(const Grid& other)
{
    if(cells == other.cells && routedWires == other.routedWires && wiresData == other.wiresData)
        return true;

    return false;
}
