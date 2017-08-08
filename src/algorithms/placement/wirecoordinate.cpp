#include "wirecoordinate.h"

WireCoordinate::WireCoordinate(Wire &wire, QList<QList<ElementCoordinate>> &elementCoordinates, QList<Library*> &libraries) :
    wire(&wire)
{
    src = nullptr;
    dest = nullptr;

    for(QList<ElementCoordinate>& list: elementCoordinates)
    {
        for(ElementCoordinate& coord: list)
        {
            if(coord.getElement().getIndex() == wire.getSrcIndex())
                src = &coord;
            if(coord.getElement().getIndex() == wire.getDestIndex())
                dest = &coord;
        }
    }

    if(src == nullptr && dest == nullptr)
        throw IllegalArgumentException(QObject::tr("Cannot find src or dest element in the given coordinate list."));

    if(src == nullptr)
    {
        src = dest;
        dest = nullptr;

        this->srcCoord = getPinCoord(src, libraries, wire.getDestPinId());
    }
    else
    {
        this->srcCoord = getPinCoord(src, libraries, wire.getSrcPinId());
    }

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
        throw Exception(QObject::tr("Cannot get destination coordinate of an external wire."));

    return QPoint(dest->getTopLeftCoord().x() + destCoord.x(), dest->getTopLeftCoord().y() + destCoord.y());
}

qint64 WireCoordinate::getFitnessValue()
{
    qint64 res = 0;

    if(position == WirePosition::External)
    {
        QPoint srcPoint = getSrcCoordinate();

        res = ((srcPoint.x() > srcPoint.y()) ? srcPoint.y() : srcPoint.x());
    }
    else
    {
        QPoint srcPoint = getSrcCoordinate();
        QPoint destPoint = getDestCoordinate();

        res = abs(srcPoint.x() - destPoint.x()) + abs(srcPoint.y() - destPoint.y()) - 1;
    }

    if(wire->getType() == WireType::Inner)
        res *= innerWireFitnessCoefficient;

    return res;
}

bool WireCoordinate::operator ==(WireCoordinate& other)
{
    return ((*wire) == (*other.getWire()));
}
