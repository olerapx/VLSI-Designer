#include "compositionalgorithmfactory.h"

CompositionAlgorithm* CompositionAlgorithmFactory::createAlgorithm(QString key, QList<Grid*> grids, Scheme* scheme)
{
    if(key == "permutation")
        return new PermutationComposition(grids, scheme);

    throw IllegalArgumentException(QString(QT_TR_NOOP("Cannot instantiate an algorithm with type %1.")).arg(key));
}
