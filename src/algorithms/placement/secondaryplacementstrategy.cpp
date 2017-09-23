#include "secondaryplacementstrategy.h"

SecondaryPlacementStrategy::SecondaryPlacementStrategy()
{
    algorithms.append(QObject::tr("Row permutation"));
}

SecondaryPlacementAlgorithm* SecondaryPlacementStrategy::createAlgorithm(int index, PlacementResult* previous)
{
    switch(index)
    {
    case 0:
        return new RowPermutationPlacement(previous);
    default:
        throw IllegalArgumentException(QObject::tr("Cannot instantiate an algorithm with type %1.").arg(index));
    }
}
