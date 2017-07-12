#ifndef PLACEMENTRESULT_H
#define PLACEMENTRESULT_H

#include <QList>
#include <QPoint>

#include "datamodels/grid/grid.h"
#include "datamodels/scheme/wire.h"

/**
 * @brief The PlacementResult class
 * Encapsulates the placement results. Can be used in secondary placement or routing.
 */
class PlacementResult
{
public:
    PlacementResult(Grid* grid, QList<QPoint>& elementTopLeftCoords, QList<Wire*>& relatedWires);

    Grid* getGrid() const { return grid; }
    QList<QPoint>& getElementTopLeftCoords() { return elementTopLeftCoords; }
    QList<Wire*>& getRelatedWires() { return relatedWires; }

private:
    Grid* grid;
    QList<QPoint> elementTopLeftCoords;
    QList<Wire*> relatedWires;
};

#endif // PLACEMENTRESULT_H
