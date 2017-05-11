#ifndef NODEELEMENT_H
#define NODEELEMENT_H

#include "datamodels/scheme/schemeelement.h"

/**
 * @brief The NodeElement class
 * Represents an element in a functional node with the given number.
 */
class NodeElement
{
private:
    SchemeElement element;
    int nodeNumber;

public:
    NodeElement(SchemeElement element, int nodeNumber);

    SchemeElement getElement() const { return element; }
    int getNodeNumber() const { return nodeNumber; }
};

#endif // NODEELEMENT_H
