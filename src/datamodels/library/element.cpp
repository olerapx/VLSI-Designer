#include "element.h"

Element::Element(QString id, int height, int width)
{
    setId(id);
    setHeight(height);
    setWidth(width);

    name = "";
    model = "";
}

Element::~Element()
{
    for (Pin* pin: pins)
        delete pin;
    pins.clear();
}

void Element::setId (QString id)
{
    if (id.isEmpty())
       throw IllegalArgumentException("Element id cannot be empty");
    this->id = id;
}

void Element::setName (QString name)
{
    this->name = name;
}

void Element::setModel (QString model)
{
    this->model = model;
}

void Element::setHeight (int height)
{
    if (height < 0)
        throw IllegalArgumentException("Element height cannot be negative");
    this->height = height;
}

void Element::setWidth (int width)
{
    if (width < 0)
        throw IllegalArgumentException ("Element width cannot be negative");
    this->width = width;
}
