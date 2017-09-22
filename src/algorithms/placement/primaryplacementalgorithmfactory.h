#pragma once

#include "algorithms/placement/rowwiseplacement/rowwiseplacement.h"

static const QList<QString> algorithmsMap
{
    { QObject::tr("Row-wise") }
};

class PrimaryPlacementAlgorithmFactory
{
public:
    static PrimaryPlacementAlgorithm* createAlgorithm(int index, Scheme* scheme, QList<Library*> libraries, double expandCoefficient);

private:
    PrimaryPlacementAlgorithmFactory() {}
};
