#include "secondaryplacementalgorithm.h"

SecondaryPlacementAlgorithm::SecondaryPlacementAlgorithm(PlacementResult* previous)
{
    setParameters(previous);
}

void SecondaryPlacementAlgorithm::setParameters(PlacementResult* previous)
{
    this->previous = previous;
}

void SecondaryPlacementAlgorithm::onStart()
{
    try
    {
        PlacementResult* result = execute();
        sendResult(result);
        sendFinish();
    }
    catch(ThreadStoppedException&)
    {
        sendFinish();
    }
    catch(Exception& e)
    {
        sendError(e.what());
        sendFinish();
    }
}

void SecondaryPlacementAlgorithm::fillWiresData(PlacementResult* result, QList<WireCoordinate> coordinates)
{
    for(WireCoordinate& c: coordinates)
    {
        if(c.getPosition() == WirePosition::Internal)
            result->getGrid()->getWiresData().append(WireData(c.getWire()->getIndex(), c.getSrcCoordinate(), c.getDestCoordinate(), c.getPosition()));
        else
            result->getGrid()->getWiresData().append(WireData(c.getWire()->getIndex(), c.getSrcCoordinate(), QPoint(0, 0), c.getPosition()));
    }
}

QList<WireCoordinate> SecondaryPlacementAlgorithm::fillWireCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates)
{
    sendLog(tr("Preparing."));

    QList<WireCoordinate> res;

    int gridHeight = previous->getGrid()->getCells().size();
    int gridWidth = previous->getGrid()->getCells()[0].size();

    for(Wire& w: previous->getRelatedWires())
    {
        res.append(WireCoordinate(w, elementCoordinates, previous->getLibraries(), gridHeight, gridWidth));
    }

    return res;
}
