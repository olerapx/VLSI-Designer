#pragma once

#include "algorithms/decomposition/serialdecomposition/schemevertex.h"

class OuterConnectionsNumberComparator
{
public:
    OuterConnectionsNumberComparator(QList<SchemeVertex*>& list);
    bool operator ()(SchemeVertex* v1, SchemeVertex* v2);

private:
    QList<SchemeVertex*>& list;
};
