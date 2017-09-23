#pragma once

#include "algorithms/routing/leerouting/leerouting.h"

class RoutingStrategy
{
public:
    RoutingStrategy();

    QList<QString>& getAlgorithms() { return algorithms; }
    RoutingAlgorithm* createAlgorithm(int index, Grid* grid, Scheme* scheme, int maxExtensionAttempts);

private:
    QList<QString> algorithms;
};
