#ifndef NODEELEMENT_H
#define NODEELEMENT_H

#include "datamodels/scheme/schemeelement.h"
#include "datamodels/scheme/wire.h"

/**
 * @brief The NodeElement class
 * Represents an element in a functional node with the given number.
 */
class NodeElement
{
private:
    SchemeElement element;
    int nodeNumber;

    QList<Wire> generatedWires;

public:
    NodeElement(SchemeElement element, int nodeNumber);

    SchemeElement getElement() const { return element; }
    int getNodeNumber() const { return nodeNumber; }

    QList<Wire>& getGeneratedWires() { return generatedWires; }
};

#endif // NODEELEMENT_H
