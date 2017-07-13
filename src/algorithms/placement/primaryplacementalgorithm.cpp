#include "primaryplacementalgorithm.h"

PrimaryPlacementAlgorithm::PrimaryPlacementAlgorithm(Scheme* scheme, double expandCoefficient)
{
    setParameters(scheme, expandCoefficient);
}

void PrimaryPlacementAlgorithm::setParameters(Scheme* scheme, double expandCoefficient)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the algorithm is working."));

    if(expandCoefficient <= 1.0)
        throw IllegalArgumentException(tr("Expanding coefficient must be bigger than 1."));

    this->scheme = scheme;
    this->expandCoefficient = expandCoefficient;
}

void PrimaryPlacementAlgorithm::onStart()
{
    try
    {
        PlacementResult* result = execute();
        sendResult(result);
        sendFinish();
    }
    catch(Exception e)
    {
        sendError(e.what());
        sendFinish();
    }
}
