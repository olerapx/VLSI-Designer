#pragma once

#include "algorithms/routing/leerouting/leerouting.h"

static const QList<QString> algorithmsMap
{
    { QObject::tr("Lee algorithm") }
};

class RoutingAlgorithmFactory
{
public:
    static RoutingAlgorithm* createAlgorithm(int index, Grid* grid, Scheme* scheme, int maxExtensionAttempts);

private:
    RoutingAlgorithmFactory() {}
};
