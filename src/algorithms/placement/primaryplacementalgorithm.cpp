#include "primaryplacementalgorithm.h"

PrimaryPlacementAlgorithm::PrimaryPlacementAlgorithm(Scheme* scheme, QList<Library*> libraries, double expandingCoefficient)
{
    setParameters(scheme, libraries, expandingCoefficient);
}

void PrimaryPlacementAlgorithm::setParameters(Scheme* scheme, QList<Library*> libraries, double expandingCoefficient)
{
    if(!stopped)
        throw Exception(tr("Cannot change parameters while the algorithm is working."));

    if(expandingCoefficient <= 1.0)
        throw IllegalArgumentException(tr("Expanding coefficient must be bigger than 1, passed: %1.").arg(QString::number(expandingCoefficient)));

    if(libraries.empty())
        throw IllegalArgumentException(tr("Cannot perform the primary placement without any element library provided."));

    this->scheme = scheme;
    this->expandingCoefficient = expandingCoefficient;
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
