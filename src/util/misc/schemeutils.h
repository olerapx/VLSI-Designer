#pragma once

#include "datamodels/scheme/scheme.h"

class SchemeUtils
{
public:
    static Wire& findWireByIndex(Scheme* s, qint64 index);
};
