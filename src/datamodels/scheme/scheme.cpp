#include "scheme.h"

Scheme::Scheme()
{

}

Scheme::~Scheme()
{
    for(SchemeElement* element: elements)
        delete element;
    elements.clear();

    for(Wire* wire: wires)
        delete wire;
    wires.clear();
}
