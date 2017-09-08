#include "compositionalgorithm.h"

CompositionAlgorithm::CompositionAlgorithm(QList<Grid*> grids)
{
    setParameters(grids);
}

void CompositionAlgorithm::setParameters(QList<Grid*> grids)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the algorithm is working."));

    this->grids = grids;
}

void CompositionAlgorithm::onStart()
{
    try
    {
        Grid* grid = execute();
        sendResult(grid);
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
