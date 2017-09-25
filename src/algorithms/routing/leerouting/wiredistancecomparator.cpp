#include "wiredistancecomparator.h"

WireDistanceComparator::WireDistanceComparator(int gridHeight, int gridWidth)
{
    this->height = gridHeight;
    this->width = gridWidth;
}

bool WireDistanceComparator::operator ()(WireData* first, WireData* second) const
{
    return (WireUtils::getDistance(first->getSrcCoord(), first->getDestCoord(), first->getWirePosition(), height, width) <
            WireUtils::getDistance(second->getSrcCoord(), second->getDestCoord(), second->getWirePosition(), height, width));
}
