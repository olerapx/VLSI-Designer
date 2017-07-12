#include "primaryplacementalgorithm.h"

PrimaryPlacementAlgorithm::PrimaryPlacementAlgorithm(Scheme* scheme, double expandCoefficient)
{
    this->scheme = scheme;

    if(expandCoefficient <= 1.0)
        throw IllegalArgumentException(tr("Expanding coefficient must be bigger than 1."));

    this->expandCoefficient = expandCoefficient;

    stopped = true;
    actuallyStopped = true;
}

void PrimaryPlacementAlgorithm::onStart()
{
    execute();
}

void PrimaryPlacementAlgorithm::onStop()
{
    stopped = true;
}
