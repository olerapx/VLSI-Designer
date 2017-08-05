#include "wirecoordinate.h"

WireCoordinate::WireCoordinate(Wire &wire, QList<ElementCoordinate> &elementCoordinates, QList<Library *> &libraries) :
    wire(&wire)
{
    src = nullptr;
    dest = nullptr;
    fitnessValue = 0;

    for(ElementCoordinate& coord: elementCoordinates)
    {
        if(coord.getElement().getIndex() == wire.getSrcIndex())
            src = &coord;
        else if(coord.getElement().getIndex() == wire.getDestIndex())
            dest = &coord;
    }

    if(src == nullptr && dest == nullptr)
        throw IllegalArgumentException("Cannot find src or dest element in the given coordinate list.");

    if(src == nullptr)
    {
        src = dest;
        dest = nullptr;
    }

    this->srcCoord = getPinCoord(src, libraries, wire.getSrcPinId());

    if(dest == nullptr)
    {
        this->position = WirePosition::External;
        return;
    }
    else
    {
        this->position = WirePosition::Internal;
        this->destCoord = getPinCoord(dest, libraries, wire.getDestPinId());
    }

    updateFitnessValue();
}

QPoint WireCoordinate::getPinCoord(ElementCoordinate *coordinate, QList<Library *> &libraries, QString pinId)
{
    LibraryElement libElement = LibraryUtils::getCorrespondingElement(coordinate->getElement(), libraries);
    Pin pin = LibraryUtils::findPinById(libElement, pinId);

    return QPoint(pin.getX(), pin.getY());
}

QPoint WireCoordinate::getSrcCoordinate()
{
    return QPoint(src->getTopLeftCoord().x() + srcCoord.x(), src->getTopLeftCoord().y() + srcCoord.y());
}

QPoint WireCoordinate::getDestCoordinate()
{
    if(position == WirePosition::External)
        throw Exception("Cannot get destination coordinate of an external wire.");

    return QPoint(dest->getTopLeftCoord().x() + destCoord.x(), dest->getTopLeftCoord().y() + destCoord.y());
}

void WireCoordinate::updateFitnessValue()
{
    fitnessValue = 0;

    if(position == WirePosition::External)
    {
        QPoint srcPoint = getSrcCoordinate();

        fitnessValue = ((srcPoint.x() > srcPoint.y()) ? srcPoint.y() : srcPoint.x());
    }
    else
    {
        QPoint srcPoint = getSrcCoordinate();
        QPoint destPoint = getDestCoordinate();

        fitnessValue = abs(srcPoint.x() - destPoint.x()) + abs(srcPoint.y() - destPoint.y());
    }

    if(wire->getType() == WireType::Inner)
        fitnessValue *= innerWireFitnessCoefficient;
}

bool WireCoordinate::operator ==(WireCoordinate& other)
{
    return ((*wire) == (*other.getWire()));
}
