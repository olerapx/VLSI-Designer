#include "compositionalgorithmfactory.h"

CompositionAlgorithm* CompositionAlgorithmFactory::createAlgorithm(int index, QList<Grid*> grids, Scheme* scheme)
{
    switch(index)
    {
    case 0:
        return new PermutationComposition(grids, scheme);
    default:
        throw IllegalArgumentException(QObject::tr("Cannot instantiate an algorithm with type %1.").arg(index));
    }
}
