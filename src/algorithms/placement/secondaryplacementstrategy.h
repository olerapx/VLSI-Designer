#pragma once

#include "algorithms/placement/rowpermutationplacement/rowpermutationplacement.h"

class SecondaryPlacementStrategy
{
public:
    SecondaryPlacementStrategy();

    QList<QString>& getAlgorithms() { return algorithms; }
    SecondaryPlacementAlgorithm* createAlgorithm(int index, PlacementResult* previous);

private:
     QList<QString> algorithms;
};
