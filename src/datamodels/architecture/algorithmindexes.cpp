#include "algorithmindexes.h"

AlgorithmIndexes::AlgorithmIndexes()
{
    compositionAlgorithmIndex = 0;
    decompositionAlgorithmIndex = 0;
    primaryPlacementAlgorithmIndex = 0;
    secondaryPlacementAlgorithmIndex = 0;
    routingAlgorithmIndex = 0;
}

void AlgorithmIndexes::setCompositionAlgorithmIndex(int index)
{
    validateIndex(index);
    this->compositionAlgorithmIndex = index;
}
void AlgorithmIndexes::setDecompositionAlgorithmIndex(int index)
{
    validateIndex(index);
    this->decompositionAlgorithmIndex = index;
}
void AlgorithmIndexes::setPrimaryPlacementAlgorithmIndex(int index)
{
    validateIndex(index);
    this->primaryPlacementAlgorithmIndex = index;
}
void AlgorithmIndexes::setSecondaryPlacementAlgorithmIndex(int index)
{
    validateIndex(index);
    this->secondaryPlacementAlgorithmIndex = index;
}
void AlgorithmIndexes::setRoutingAlgorithmIndex(int index)
{
    validateIndex(index);
    this->routingAlgorithmIndex = index;
}

void AlgorithmIndexes::validateIndex(int index)
{
    if(index < 0)
        throw IllegalArgumentException(QObject::tr("Algorithm index cannot be negative, passed: %1.").arg(QString::number(index)));
}

bool AlgorithmIndexes::operator ==(const AlgorithmIndexes& other) const
{
    if(compositionAlgorithmIndex == other.compositionAlgorithmIndex &&
            decompositionAlgorithmIndex == other.decompositionAlgorithmIndex &&
            primaryPlacementAlgorithmIndex == other.primaryPlacementAlgorithmIndex &&
            secondaryPlacementAlgorithmIndex == other.secondaryPlacementAlgorithmIndex &&
            routingAlgorithmIndex == other.routingAlgorithmIndex)
        return true;

    return false;
}
