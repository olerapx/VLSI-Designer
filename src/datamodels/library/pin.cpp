#include "pin.h"

Pin::Pin(QString id, int x, int y, PinType type)
{
    setId(id);
    setX(x);
    setY(y);
    setType(type);
}

void Pin::setId(QString id)
{
    if (id.isEmpty())
       throw IllegalArgumentException(QObject::tr("Pin id cannot be empty."));
    this->id = id;
}

void Pin::setX(int x)
{
    if (x < 0)
        throw IllegalArgumentException (QObject::tr("Pin coordinate cannot be negative, passed: %1.").arg(QString::number(x)));
    this->x = x;
}

void Pin::setY(int y)
{
    if (y < 0)
        throw IllegalArgumentException (QObject::tr("Pin coordinate cannot be negative, passed: %1.").arg(QString::number(y)));
    this->y = y;
}

void Pin::setType(PinType type)
{
    this->type = type;
}

bool Pin::operator ==(const Pin& other) const
{
    if(id == other.id && x == other.x && y == other.y && type == other.type)
        return true;

    return false;
}
