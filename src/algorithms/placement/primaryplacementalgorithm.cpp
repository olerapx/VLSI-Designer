#include "primaryplacementalgorithm.h"

PrimaryPlacementAlgorithm::PrimaryPlacementAlgorithm(Scheme* scheme, QList<Library*> libraries, double expandCoefficient)
{
    setParameters(scheme, libraries, expandCoefficient);
}

void PrimaryPlacementAlgorithm::setParameters(Scheme* scheme, QList<Library*> libraries, double expandCoefficient)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the algorithm is working."));

    if(expandCoefficient <= 1.0)
        throw IllegalArgumentException(tr("Expanding coefficient must be bigger than 1, passed: %1.").arg(QString::number(expandCoefficient)));

    if(libraries.empty())
        throw IllegalArgumentException(tr("Cannot perform the primary placement without any element library provided."));

    this->scheme = scheme;
    this->expandCoefficient = expandCoefficient;
    this->libraries = libraries;
}

void PrimaryPlacementAlgorithm::onStart()
{
    try
    {
        PlacementResult* result = execute();
        sendResult(result);
        sendFinish();
    }
    catch(ThreadStoppedException tse)
    {
        sendFinish();
    }
    catch(Exception e)
    {
        sendError(e.what());
        sendFinish();
    }
}
