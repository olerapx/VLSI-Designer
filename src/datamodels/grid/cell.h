#ifndef CELL_H
#define CELL_H

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"

enum class CellType
{
    Empty,
    Pin,
    Element,
    UD, /// Vertical wire.
    LR, /// Horizontal wire.
    UL, UR, DL, DR, /// Corner wires.
    UDL, UDR, LRU, LRD, /// Crossing corner wires.
    UDLR /// Crossing wires.
};

/**
 * @brief The Cell class
 * Represents a grid's cell.
 */
class Cell: public Serializable
{
protected:
    CellType type;
    qint64 index; /// Applies only on an element or a pin.
    QString pinId; /// Applies only on a pin cell.

public:
    Cell (CellType type, qint64 index = -1, QString pinId = "");
    virtual ~Cell() {}

    void setType (CellType type, qint64 index = -1, QString pinId = "");
    void setIndex (qint64 index);
    void setPinId (QString pinId);

    CellType getType() const { return type; }
    qint64 getIndex() const { return index; }
    QString getPinId() const { return pinId; }
};

#endif // CELL_H
