#pragma once

#include "algorithms/decomposition/serialdecomposition/serialdecomposition.h"

class DecompositionStrategy
{
public:
    DecompositionStrategy();

    QList<QString>& getAlgorithms() { return algorithms; }
    DecompositionAlgorithm* createAlgorithm(int index, Scheme* scheme, int number);

private:
    QList<QString> algorithms;
};
