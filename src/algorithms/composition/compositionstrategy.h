#pragma once

#include "algorithms/composition/permutationcomposition/permutationcomposition.h"

class CompositionStrategy
{
public:
   CompositionStrategy();

   QList<QString>& getAlgorithms() { return algorithms; }
   CompositionAlgorithm* createAlgorithm(int index, QList<Grid*> grids, Scheme* scheme);

private:
   QList<QString> algorithms;
};
