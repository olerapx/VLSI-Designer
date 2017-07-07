#include "decompositionalgorithm.h"

DecompositionAlgorithm::DecompositionAlgorithm(Scheme *scheme, int number)
{
    this->scheme = scheme;
    this->number = number;

    stopped = true;
    actuallyStopped = true;
}

void DecompositionAlgorithm::onStart()
{
    execute();
}

void DecompositionAlgorithm::onStop()
{
    stopped = true;
}
