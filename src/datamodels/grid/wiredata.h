#pragma once

#include <QPoint>

#include "datamodels/serializable.h"

enum class WirePosition
{
    Internal,
    External
};

/**
 * @brief The WireData class
 * Contains the source and destination coordinates of the given wire.
 */
class WireData : public Serializable
{
protected:
    qint64 index;

    int srcX;
    int srcY;

    int destX;
    int destY;

    WirePosition position;

public:
    WireData(qint64 index, QPoint srcCoord, QPoint destCoord, WirePosition position);

    void setIndex(qint64 index);

    void setSrcCoord(QPoint srcCoord);
    void setDestCoord(QPoint destCoord);

    void setWirePosition(WirePosition position);

    qint64 getIndex() const { return index; }

    QPoint getSrcCoord();
    QPoint getDestCoord();

    WirePosition getWirePosition() const { return position; }

    bool operator ==(const WireData& other) const;
};
