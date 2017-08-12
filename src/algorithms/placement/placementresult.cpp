#include "placementresult.h"

PlacementResult::PlacementResult(Grid* grid, QList<ElementCoordinate>& elementCoordinates, QList<Wire>& relatedWires, QList<Library*> libraries) :
    grid(grid),
    elementCoordinates(elementCoordinates),
    relatedWires(relatedWires),
    libraries(libraries)
{

}

QList<QList<ElementCoordinate>> PlacementResult::getRowWiseCoordinates()
{
    QList<QList<ElementCoordinate>> res;
    res.append(QList<ElementCoordinate>());
    int lastY = 0;
    bool firstTime = true;

    for(ElementCoordinate coord: elementCoordinates)
    {
        QPoint point = coord.getTopLeftCoord();
        if(point.y() != lastY && !firstTime)
           res.append(QList<ElementCoordinate>());

        res.last().append(coord);

        firstTime = false;
        lastY = point.y();
    }

    return res;
}
