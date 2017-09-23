#pragma once

#include "algorithms/placement/rowwiseplacement/rowwiseplacement.h"

class PrimaryPlacementStrategy
{
public:
    PrimaryPlacementStrategy();

    QList<QString>& getAlgorithms() { return algorithms; }
    PrimaryPlacementAlgorithm* createAlgorithm(int index, Scheme* scheme, QList<Library*> libraries, double expandCoefficient);

private:
    QList<QString> algorithms;
};
