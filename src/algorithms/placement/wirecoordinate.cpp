#include "wirecoordinate.h"

WireCoordinate::WireCoordinate(Wire& wire, QList<QList<ElementCoordinate>>& elementCoordinates, QList<Library*>& libraries, int gridHeight, int gridWidth) :
    wire(&wire)
{
    src = nullptr;
    dest = nullptr;

    height = gridHeight;
    width = gridWidth;

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
        throw IllegalArgumentException(QObject::tr("Cannot find source (index: %1) or destination (index: %2) element in the given coordinate list.")
                                       .arg(QString::number(wire.getSrcIndex()), QString::number(wire.getDestIndex())));

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

QPoint WireCoordinate::getPinCoord(ElementCoordinate* coordinate, QList<Library*>& libraries, QString pinId)
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
        res = WireUtils::getDistance(getSrcCoordinate(), QPoint(0, 0), position, height, width);
    else
        res = WireUtils::getDistance(getSrcCoordinate(), getDestCoordinate(), position, height, width);

    if(wire->getType() == WireType::InNode)
        res *= inNodeWireFitnessCoefficient;

    return res;
}

bool WireCoordinate::operator ==(WireCoordinate& other) const
{
    return ((*wire) == (*other.wire));
}
