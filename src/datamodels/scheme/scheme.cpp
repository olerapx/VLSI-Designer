#include "scheme.h"

bool Scheme::operator ==(const Scheme& other)
{
    if(elements == other.elements && wires == other.wires)
        return true;

    return false;
}
