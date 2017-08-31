#ifndef NODEELEMENT_H
#define NODEELEMENT_H

#include "datamodels/scheme/schemeelement.h"
#include "util/misc/libraryutils.h"

/**
 * @brief The NodeElement class
 * Represents an element in a functional node with the given number.
 */
class NodeElement
{
private:
    SchemeElement element;
    int nodeNumber;

    QList<Pin> availableInputPins;

public:
    NodeElement(SchemeElement element, int nodeNumber, QList<Library*>& libraries);

    SchemeElement getElement() const { return element; }
    int getNodeNumber() const { return nodeNumber; }

    QList<Pin>& getAvailableInputPins() { return availableInputPins; }
};

#endif // NODEELEMENT_H
