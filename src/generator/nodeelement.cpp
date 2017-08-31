#include "nodeelement.h"

NodeElement::NodeElement(SchemeElement element, int nodeNumber, QList<Library *> &libraries)
    : element(element)
{
    this->nodeNumber = nodeNumber;

    LibraryElement libElement = LibraryUtils::getCorrespondingElement(element, libraries);

    for(Pin& p: libElement.getPins())
    {
        if(p.getType() == PinType::Input)
            availableInputPins.append(p);
    }
}
