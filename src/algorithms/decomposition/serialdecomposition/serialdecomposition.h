#pragma once

#include "algorithms/decomposition/decompositionalgorithm.h"
#include "algorithms/decomposition/serialdecomposition/schemevertex.h"
#include "algorithms/decomposition/serialdecomposition/outerconnectionsnumbercomparator.h"

/**
 * @brief The SerialDecomposition class
 * Performs the serial decomposition of the given scheme on the given number of parts.
 */
class SerialDecomposition : public DecompositionAlgorithm
{
public:
    SerialDecomposition(Scheme* scheme, int number);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @throw Exception
     * @return decomposed scheme.
     */
    QList<Scheme*> execute();

private:
    QList<SchemeVertex*> undistributedVertices;
    QList<QList<SchemeVertex*>> distributedVertices;

    void clear();

    void fillVertices();
    SchemeVertex* findVertexByIndex(qint64 index);

    void prepareSubschemes();

    void fillSubscheme(QList<SchemeVertex*>& list);

    void placeFirstElement(QList<SchemeVertex*>& list);
    static bool connectedElementsNumberComparator(SchemeVertex* v1, SchemeVertex* v2);

    void placeNextElement(QList<SchemeVertex*>& list, int index);
    QList<SchemeVertex*> getSelectableVertices(QList<SchemeVertex*>& list);

    QList<Scheme*> buildSubschemes();
};
