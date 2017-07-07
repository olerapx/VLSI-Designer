#include "schemevertex.h"

SchemeVertex::SchemeVertex(SchemeElement* element)
{
    this->element = element;
    this->distributed = false;
}

void SchemeVertex::setDistributed(bool distributed)
{
    this->distributed = distributed;
}

bool SchemeVertex::operator ==(SchemeVertex* other)
{
    return (element == other->getElement());
}
