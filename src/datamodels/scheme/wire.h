#ifndef WIRE_H
#define WIRE_H

#include "src/datamodels/serializable.h"
#include "src/exception/illegalargumentexception.h"

enum class WireType
{
    Inner,
    Outer
};

/**
 * @brief The Wire class
 * Represents a wire on a generated scheme.
 */
class Wire: public Serializable
{
protected:
    qint64 srcIndex;
    QString srcPinId;

    qint64 destIndex;
    QString destPinId;

    WireType type;
    qint64 index;

public:
    Wire(int srcIndex, QString srcPinId, int destIndex, QString destPinId, WireType type, int index);

    void setSrcIndex(qint64 srcIndex);
    void setSrcPinId(QString srcPinId);

    void setDestIndex(qint64 destIndex);
    void setDestPinId(QString destPinId);

    void setType(WireType type);
    void setIndex(qint64 index);


    qint64 getSrcIndex() {return this->srcIndex;}
    QString getSrcPinId() {return this->srcPinId;}

    qint64 getDestIndex() {return this->destIndex;}
    QString getDestPinId() {return this->destPinId;}

    WireType getType() {return this->type;}
    qint64 getIndex() {return this->index;}
};

#endif // WIRE_H
