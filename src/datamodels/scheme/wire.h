#pragma once

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"

enum class WireType
{
    Inner,
    Outer
};

/**
 * @brief The Wire class
 * Represents a wire on a generated scheme.
 */
class Wire : public Serializable
{
protected:
    qint64 srcIndex;
    QString srcPinId;

    qint64 destIndex;
    QString destPinId;

    WireType type;
    qint64 index;

public:
    Wire(qint64 srcIndex, QString srcPinId, qint64 destIndex, QString destPinId, WireType type, qint64 index);
    virtual ~Wire() {}

    void setSrcIndex(qint64 srcIndex);
    void setSrcPinId(QString srcPinId);

    void setDestIndex(qint64 destIndex);
    void setDestPinId(QString destPinId);

    void setType(WireType type);
    void setIndex(qint64 index);

    qint64 getSrcIndex() const { return srcIndex; }
    QString getSrcPinId() const { return srcPinId; }

    qint64 getDestIndex() const { return destIndex; }
    QString getDestPinId() const { return destPinId; }

    WireType getType() const { return type; }
    qint64 getIndex() const { return index; }

    /**
     * @brief operator ==
     * @param other
     * @return true if wires are equal regardless to their indexes, false otherwise.
     */
    bool operator ==(const Wire& other);
};
