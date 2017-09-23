#include "primaryplacementstrategy.h"

PrimaryPlacementStrategy::PrimaryPlacementStrategy()
{
    algorithms.append(QObject::tr("Row-wise"));
}

PrimaryPlacementAlgorithm* PrimaryPlacementStrategy::createAlgorithm(int index, Scheme* scheme, QList<Library*> libraries, double expandCoefficient)
{
    switch(index)
    {
    case 0:
        return new RowWisePlacement(scheme, libraries, expandCoefficient);
    default:
        throw IllegalArgumentException(QObject::tr("Cannot instantiate an algorithm with type %1.").arg(index));
    }
}
