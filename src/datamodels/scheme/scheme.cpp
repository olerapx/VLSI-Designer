#include "scheme.h"

Scheme::Scheme()
{

}

Scheme::~Scheme()
{
    foreach(SchemeElement* element, elements)
        delete element;
    elements.clear();

    foreach(Wire* wire, wires)
        delete wire;
    wires.clear();
}
