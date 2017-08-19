#include "wire.h"

Wire::Wire(qint64 srcIndex, QString srcPinId, qint64 destIndex, QString destPinId, WireType type, qint64 index)
{
    setSrcIndex(srcIndex);
    setSrcPinId(srcPinId);

    setDestIndex(destIndex);
    setDestPinId(destPinId);

    setType(type);
    setIndex(index);
}

void Wire::setSrcIndex(qint64 srcIndex)
{
    if(srcIndex < 0)
        throw IllegalArgumentException(QObject::tr("Wire source index cannot be negative, passed: %1.")
                                       .arg(QString::number(srcIndex)));
    this->srcIndex = srcIndex;
}

void Wire::setSrcPinId(QString srcPinId)
{
    if(srcPinId.isEmpty())
        throw IllegalArgumentException(QObject::tr("Wire source pin id cannot be empty."));
    this->srcPinId = srcPinId;
}

void Wire::setDestIndex(qint64 destIndex)
{
    if(destIndex < 0)
        throw IllegalArgumentException(QObject::tr("Wire destination index cannot be negative, passed: %1.")
                                       .arg(QString::number(destIndex)));
    this->destIndex = destIndex;
}

void Wire::setDestPinId(QString destPinId)
{
    if(destPinId.isEmpty())
        throw IllegalArgumentException(QObject::tr("Wire destination pin id cannot be empty."));
    this->destPinId = destPinId;
}

void Wire::setType(WireType type)
{
    this->type = type;
}

void Wire::setIndex(qint64 index)
{
    if(index < 0)
        throw IllegalArgumentException(QObject::tr("Wire index cannot be negative, passed: %1.").arg(QString::number(index)));
    this->index = index;
}

bool Wire::operator ==(const Wire& other)
{
    if(srcIndex == other.srcIndex && srcPinId == other.srcPinId &&
            destIndex == other.getDestIndex() && destPinId == other.getDestPinId() &&
            type == other.getType())
        return true;

    return false;
}
