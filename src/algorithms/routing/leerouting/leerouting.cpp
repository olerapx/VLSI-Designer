#include "leerouting.h"

LeeRouting::LeeRouting(Grid *grid, Scheme *scheme) :
    RoutingAlgorithm(grid, scheme)
{
    clear();
}

Grid* LeeRouting::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        //      working

        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

        stopped = true;
        actuallyStopped = true;

        return grid;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void LeeRouting::clear()
{

}
