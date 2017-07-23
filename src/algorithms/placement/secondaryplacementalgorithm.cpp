#include "secondaryplacementalgorithm.h"

SecondaryPlacementAlgorithm::SecondaryPlacementAlgorithm(PlacementResult *previous)
{
    setParameters(previous);
}

void SecondaryPlacementAlgorithm::setParameters(PlacementResult *previous)
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
    catch(ThreadStoppedException& tse)
    {
        sendFinish();
    }
    catch(Exception e)
    {
        sendError(e.what());
        sendFinish();
    }
}
