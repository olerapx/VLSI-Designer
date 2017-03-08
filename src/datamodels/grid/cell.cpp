#include "cell.h"

Cell::Cell(CellType type, qint64 index, QString pinId)
{
    setType(type, index, pinId);
}

void Cell::setType (CellType type, qint64 index, QString pinId)
{
    setType(type);

    if (type != CellType::Element && index == 0)
        throw IllegalArgumentException ("Cant create element cell without index");
    if (type != CellType::Pin && pinId == "")
        throw IllegalArgumentException ("Cant create pin cell without pinId");

    if (type == CellType::Element)
    {
        setIndex(index);
    }
    else if (type == CellType::Pin)
    {
        setIndex(index);
        setPinId(pinId);
    }
}

void Cell::setIndex (qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException ("Element index cannot be negative");
    this->index = index;
}

void Cell::setPinId (QString pinId)
{
    if (pinId.isEmpty())
        throw IllegalArgumentException ("Pin id cannot be empty");
    this->pinId = pinId;
}
