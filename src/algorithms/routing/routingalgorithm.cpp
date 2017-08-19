#include "routingalgorithm.h"

RoutingAlgorithm::RoutingAlgorithm(Grid* grid, Scheme* scheme)
{
    setParameters(grid, scheme);
}

void RoutingAlgorithm::setParameters(Grid *grid, Scheme *scheme)
{
    this->grid = grid;
    this->scheme = scheme;
}

void RoutingAlgorithm::onStart()
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
