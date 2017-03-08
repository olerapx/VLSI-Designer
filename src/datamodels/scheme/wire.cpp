#include "wire.h"

Wire::Wire(int srcIndex, QString srcPinId, int destIndex, QString destPinId, WireType type, int index)
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
    if (srcIndex < 0)
        throw IllegalArgumentException("Wire source index cannot be negative");
    this->srcIndex = srcIndex;
}

void Wire::setSrcPinId(QString srcPinId)
{
    if (srcPinId.isEmpty())
        throw IllegalArgumentException("Wire source pin id cannot be empty");
    this->srcPinId = srcPinId;
}

void Wire::setDestIndex(qint64 destIndex)
{
    if (destIndex < 0)
        throw IllegalArgumentException("Wire destination index cannot be negative");
    this->destIndex = destIndex;
}

void Wire::setDestPinId(QString destPinId)
{
    if (destPinId.isEmpty())
        throw IllegalArgumentException("Wire destination pin id cannot be empty");
    this->destPinId = destPinId;
}

void Wire::setType(WireType type)
{
    this->type = type;
}

void Wire::setIndex(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException("Wire index cannot be negative");
    this->index = index;
}
