#include "placementresult.h"

PlacementResult::PlacementResult(Grid *grid, QList<QPoint> &elementTopLeftCoords, QList<Wire *> &relatedWires)
{
    this->grid = grid;
    this->elementTopLeftCoords = elementTopLeftCoords;
    this->relatedWires = relatedWires;
}
