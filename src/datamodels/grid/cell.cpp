#include "cell.h"

Cell::Cell(CellType type, qint64 index, QString pinId)
{
    setType(type, index, pinId);
}

void Cell::setType(CellType type, qint64 index, QString pinId)
{
    this->type = type;
    this->index = 0;

    if (type == CellType::Element)
    {
        if (index == -1)
            throw IllegalArgumentException("Cant create element cell without index");

        setIndex(index);

        return;
    }
    if (type == CellType::Pin)
    {
        if (pinId.isEmpty())
            throw IllegalArgumentException("Cant create pin cell without pinId");

        setIndex(index);
        setPinId(pinId);

        return;
    }
}

void Cell::setIndex(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException("Element index cannot be negative");
    this->index = index;
}

void Cell::setPinId(QString pinId)
{
    if (pinId.isEmpty())
        throw IllegalArgumentException("Pin id cannot be empty");
    this->pinId = pinId;
}

bool Cell::operator ==(const Cell& other)
{
    if(type != other.type)
        return false;

    if(type == CellType::Element)
    {
        if(index == other.index) return true;
        else return false;
    }

    if(type == CellType::Pin)
    {
        if(index == other.index && pinId == other.pinId) return true;
        else return false;
    }

    return true;
}
