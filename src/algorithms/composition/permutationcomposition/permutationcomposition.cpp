#include "permutationcomposition.h"

PermutationComposition::PermutationComposition(QList<Grid*> grids) :
    CompositionAlgorithm(grids)
{
    clear();
}

Grid* PermutationComposition::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        //working
        Grid* grid;
        //working

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

void PermutationComposition::clear()
{

}
