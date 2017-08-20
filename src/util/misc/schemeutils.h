#ifndef SCHEMEUTILS_H
#define SCHEMEUTILS_H

#include "datamodels/scheme/scheme.h"

class SchemeUtils
{
public:
    static Wire& findWireByIndex(Scheme* s, qint64 index);
};

#endif // SCHEMEUTILS_H
