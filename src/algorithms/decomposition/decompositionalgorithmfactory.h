#pragma once

#include "algorithms/decomposition/serialdecomposition/serialdecomposition.h"

static const QList<QString> algorithms
{
    { QObject::tr("Serial") }
};

class DecompositionAlgorithmFactory
{
public:
    static DecompositionAlgorithm* createAlgorithm(int index, Scheme* scheme, int number);

private:
    DecompositionAlgorithmFactory() {}
};
