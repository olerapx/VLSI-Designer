#include "libraryelement.h"

LibraryElement::LibraryElement(QString id, int height, int width)
{
    setId(id);
    setHeight(height);
    setWidth(width);

    name = "";
    model = "";
}

void LibraryElement::setId(QString id)
{
    if (id.isEmpty())
       throw IllegalArgumentException(QObject::tr("Element id cannot be empty."));
    this->id = id;
}

void LibraryElement::setName(QString name)
{
    this->name = name;
}

void LibraryElement::setModel(QString model)
{
    this->model = model;
}

void LibraryElement::setHeight(int height)
{
    if (height <= 0)
        throw IllegalArgumentException(QObject::tr("Element height must be positive, passed: %1.").arg(QString::number(height)));
    this->height = height;
}

void LibraryElement::setWidth(int width)
{
    if (width <= 0)
        throw IllegalArgumentException(QObject::tr("Element width must be positive, passed: %1.").arg(QString::number(width)));
    this->width = width;
}

bool LibraryElement::operator ==(const LibraryElement& other) const
{
    if(id == other.id && name == other.name && model == other.model &&
            height == other.height && width == other.width &&
            pins == other.pins)
        return true;

    return false;
}
