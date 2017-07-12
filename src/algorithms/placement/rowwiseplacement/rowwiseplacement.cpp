#include "rowwiseplacement.h"

RowWisePlacement::RowWisePlacement(Scheme *scheme, double expandCoefficient):
    PrimaryPlacementAlgorithm(scheme, expandCoefficient)
{

}

PlacementResult* RowWisePlacement::execute()
{
    if(!actuallyStopped)
    {
        sendError(tr("The algorithm is already working."));
        return nullptr;
    }

    stopped = false;
    actuallyStopped = false;

    try
    {
        PlacementResult* res;

        stopped = true;
        actuallyStopped = true;

        sendResult(res);
        sendFinish();

        return res;
    }
    catch(Exception e)
    {
        stopped = true;
        actuallyStopped = true;

        sendError(e.what());
        sendFinish();
        return nullptr;
    }
}
