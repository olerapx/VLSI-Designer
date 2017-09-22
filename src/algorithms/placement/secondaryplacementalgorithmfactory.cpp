#include "secondaryplacementalgorithmfactory.h"

SecondaryPlacementAlgorithm* SecondaryPlacementAlgorithmFactory::createAlgorithm(int index, PlacementResult* previous)
{
    switch(index)
    {
    case 0:
        return new RowPermutationPlacement(previous);
    default:
        throw IllegalArgumentException(QObject::tr("Cannot instantiate an algorithm with type %1.").arg(index));
    }
}
