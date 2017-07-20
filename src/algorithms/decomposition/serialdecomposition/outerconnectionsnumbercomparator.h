#ifndef OUTERCONNECTIONSNUMBERCOMPARATOR_H
#define OUTERCONNECTIONSNUMBERCOMPARATOR_H

#include "algorithms/decomposition/serialdecomposition/schemevertex.h"

class OuterConnectionsNumberComparator
{
public:
    OuterConnectionsNumberComparator(QList<SchemeVertex*>& list);
    bool operator ()(SchemeVertex* v1, SchemeVertex* v2);

private:
    QList<SchemeVertex*>& list;
};

#endif // OUTERCONNECTIONSNUMBERCOMPARATOR_H
