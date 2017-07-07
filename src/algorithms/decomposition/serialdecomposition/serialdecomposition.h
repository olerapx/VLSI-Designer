#ifndef SERIALDECOMPOSITION_H
#define SERIALDECOMPOSITION_H

#include "algorithms/decomposition/decompositionalgorithm.h"
#include "algorithms/decomposition/serialdecomposition/schemevertex.h"
#include "algorithms/decomposition/serialdecomposition/outerconnectionsnumbercomparator.h"

class SerialDecomposition: public DecompositionAlgorithm
{
public:
    SerialDecomposition(Scheme* scheme, int number);

    QList<Scheme*> execute();

private:
    QList<SchemeVertex*> undistributedVertices;
    QList<QList<SchemeVertex*>> distributedVertices;
    OuterConnectionsNumberComparator outerConnectionsNumberComparator;

    void clear();

    void fillVertices();
    SchemeVertex* findVertexByIndex(qint64 index);

    void prepareSubschemes();

    void fillSubscheme(QList<SchemeVertex*>& list);

    void placeFirstElement(QList<SchemeVertex*>& list);
    static bool connectedElementsNumberComparator(SchemeVertex* v1, SchemeVertex* v2);

    void placeNextElement(QList<SchemeVertex*>& list, int index);
    QList<SchemeVertex*> getSelectableVertices(QList<SchemeVertex*>& list);   
};

#endif // SERIALDECOMPOSITION_H
