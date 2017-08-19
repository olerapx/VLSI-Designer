#include "wiredata.h"

WireData::WireData(qint64 index, QPoint srcCoord, QPoint destCoord, WirePosition position)
{
    setIndex(index);

    setSrcCoord(srcCoord);

    if(position == WirePosition::External)
        setDestCoord(QPoint(0, 0));
    else
        setDestCoord(destCoord);

    setWirePosition(position);
}

void WireData::setIndex(qint64 index)
{
    if(index < 0)
        throw IllegalArgumentException(QObject::tr("Wire index cannot be negative, passed: %1.").arg(QString::number(index)));

    this->index = index;
}

void WireData::setSrcCoord(QPoint srcCoord)
{
    if(srcCoord.x() < 0 || srcCoord.y() < 0)
        throw IllegalArgumentException(QObject::tr("Wire source coordinates cannot be negative, passed: (%1; %2).")
                                       .arg(QString::number(srcCoord.x()), QString::number(srcCoord.y())));

    this->srcX = srcCoord.x();
    this->srcY = srcCoord.y();
}

void WireData::setDestCoord(QPoint destCoord)
{
    if(destCoord.x() < 0 || destCoord.y() < 0)
        throw IllegalArgumentException(QObject::tr("Wire destination coordinates cannot be negative, passed: (%1; %2).")
                                       .arg(QString::number(destCoord.x()), QString::number(destCoord.y())));

    this->destX = destCoord.x();
    this->destY = destCoord.y();
}

void WireData::setWirePosition(WirePosition position)
{
    this->position = position;
}

QPoint WireData::getSrcCoord()
{
    return QPoint(srcX, srcY);
}

QPoint WireData::getDestCoord()
{
    if(position == WirePosition::External)
        throw Exception(QObject::tr("Cannot get destination coordinate of an external wire."));

    return QPoint(destX, destY);
}

bool WireData::operator ==(const WireData& other)
{
    if(index == other.index && srcX == other.srcX && srcY == other.srcY &&
            destX == other.destX && destY == other.destY &&
            position == other.position)
        return true;

    return false;
}
