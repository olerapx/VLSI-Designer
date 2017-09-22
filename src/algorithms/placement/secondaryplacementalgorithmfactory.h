#pragma once

#include "algorithms/placement/rowpermutationplacement/rowpermutationplacement.h"

static const QList<QString> algorithmsMap
{
    { QObject::tr("Row permutation") }
};

class SecondaryPlacementAlgorithmFactory
{
public:
    static SecondaryPlacementAlgorithm* createAlgorithm(int index, PlacementResult* previous);

private:
    SecondaryPlacementAlgorithmFactory() {}
};
