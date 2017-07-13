#include "rowwiseplacement.h"

RowWisePlacement::RowWisePlacement(Scheme *scheme, double expandCoefficient):
    PrimaryPlacementAlgorithm(scheme, expandCoefficient)
{

}

PlacementResult* RowWisePlacement::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        PlacementResult* res;

        stopped = true;
        actuallyStopped = true;

        return res;
    }
    catch(Exception e)
    {
        stopped = true;
        actuallyStopped = true;

        throw;
    }
}
