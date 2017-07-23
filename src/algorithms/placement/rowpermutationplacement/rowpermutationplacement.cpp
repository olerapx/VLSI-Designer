#include "rowpermutationplacement.h"

RowPermutationPlacement::RowPermutationPlacement(PlacementResult* previous):
    SecondaryPlacementAlgorithm(previous)
{
    clear();
}

PlacementResult* RowPermutationPlacement::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        PlacementResult* res = nullptr;
        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

        stopped = true;
        actuallyStopped = true;

        return res;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void RowPermutationPlacement::clear()
{

}
