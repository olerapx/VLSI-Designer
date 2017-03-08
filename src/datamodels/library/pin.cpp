#include "pin.h"

Pin::Pin(QString id, int x, int y, PinType type)
{
    setId (id);
    setX (x);
    setY (y);
    setType (type);
}

void Pin::setId (QString id)
{
    if (id.isEmpty())
       throw IllegalArgumentException("Pin id cannot be empty");
    this->id = id;
}

void Pin::setX (int x)
{
    if (x < 0)
        throw IllegalArgumentException ("Pin coordinate cannot be negative");
    this->x = x;
}

void Pin::setY (int y)
{
    if (y < 0)
        throw IllegalArgumentException ("Pin coordinate cannot be negative");
    this->y = y;
}

void Pin::setType (PinType type)
{
    this->type = type;
}
