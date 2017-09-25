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
            throw IllegalArgumentException(QObject::tr("Cannot create element cell without index."));

        setIndex(index);

        return;
    }
    if (type == CellType::Pin)
    {
        if (pinId.isEmpty())
            throw IllegalArgumentException(QObject::tr("Cannot create pin cell without pinId."));

        setIndex(index);
        setPinId(pinId);

        return;
    }
}

void Cell::setIndex(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException(QObject::tr("Element index cannot be negative, passed: %1.").arg(QString::number(index)));
    this->index = index;
}

void Cell::setPinId(QString pinId)
{
    if (pinId.isEmpty())
        throw IllegalArgumentException(QObject::tr("Pin id cannot be empty."));
    this->pinId = pinId;
}

bool Cell::operator ==(const Cell& other) const
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

bool Cell::isWire() const
{
    return (type != CellType::Element && type != CellType::Pin && type != CellType::Empty);
}
