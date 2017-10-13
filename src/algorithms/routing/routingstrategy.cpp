#include "routingstrategy.h"

RoutingStrategy::RoutingStrategy()
{
    algorithms.append(QObject::tr("Lee algorithm"));
}

RoutingAlgorithm* RoutingStrategy::createAlgorithm(int index, Grid* grid, Scheme* scheme, int maxExtensionAttempts, int maxExtensionAttemptsForWire)
{
    switch(index)
    {
    case 0:
        return new LeeRouting(grid, scheme, maxExtensionAttempts, maxExtensionAttemptsForWire);
    default:
        throw IllegalArgumentException(QObject::tr("Cannot instantiate an algorithm with type %1.").arg(index));
    }
}
