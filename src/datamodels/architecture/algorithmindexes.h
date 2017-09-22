#pragma once

#include <QObject>

#include "exception/illegalargumentexception.h"

/**
 * @brief The AlgorithmIndexes class
 * Encapsulates the indexes of algorithms used in design process.
 */
class AlgorithmIndexes
{
public:
    AlgorithmIndexes();

    void setCompositionAlgorithmIndex(int index);
    void setDecompositionAlgorithmIndex(int index);
    void setPrimaryPlacementAlgorithmIndex(int index);
    void setSecondaryPlacementAlgorithmIndex(int index);
    void setRoutingAlgorithmIndex(int index);

    int getCompositionAlgorithmIndex() { return compositionAlgorithmIndex; }
    int getDecompositionAlgorithmIndex() { return decompositionAlgorithmIndex; }
    int getPrimaryPlacementAlgorithmIndex() { return primaryPlacementAlgorithmIndex; }
    int getSecondaryPlacementAlgorithmIndex() { return secondaryPlacementAlgorithmIndex; }
    int getRoutingAlgorithmIndex() { return routingAlgorithmIndex; }

    bool operator ==(const AlgorithmIndexes& other);

private:
    void validateIndex(int index);

    int compositionAlgorithmIndex;
    int decompositionAlgorithmIndex;
    int primaryPlacementAlgorithmIndex;
    int secondaryPlacementAlgorithmIndex;
    int routingAlgorithmIndex;
};
