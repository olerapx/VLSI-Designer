#pragma once

#include "algorithms/composition/permutationcomposition/permutationcomposition.h"

static const QList<QString> algorithms
{
    { QObject::tr("Permutation") }
};

class CompositionAlgorithmFactory
{
public:
   static CompositionAlgorithm* createAlgorithm(int index, QList<Grid*> grids, Scheme* scheme);

private:
   CompositionAlgorithmFactory() {}
};
