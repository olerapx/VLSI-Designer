#ifndef PLACEMENTRESULT_H
#define PLACEMENTRESULT_H

#include <QList>

#include "elementcoordinate.h"
#include "datamodels/grid/grid.h"
#include "datamodels/scheme/wire.h"
#include "datamodels/library/library.h"

/**
 * @brief The PlacementResult class
 * Encapsulates the placement results. Can be used in secondary placement or routing.
 */
class PlacementResult
{
public:
    PlacementResult(Grid* grid, QList<ElementCoordinate> &elementCoordinates, QList<Wire>& relatedWires, QList<Library*> libraries);

    Grid* getGrid() const { return grid; }
    QList<ElementCoordinate>& getElementCoordinates() { return elementCoordinates; }
    QList<QList<ElementCoordinate>> getRowWiseCoordinates();
    QList<Wire>& getRelatedWires() { return relatedWires; }
    QList<Library*>& getLibraries() { return libraries; }

private:
    Grid* grid;
    QList<ElementCoordinate> elementCoordinates;
    QList<Wire> relatedWires;
    QList<Library*> libraries;
};

#endif // PLACEMENTRESULT_H
