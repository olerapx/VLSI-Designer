#include "libraryelement.h"

LibraryElement::LibraryElement(QString id, int height, int width)
{
    setId(id);
    setHeight(height);
    setWidth(width);

    name = "";
    model = "";
}

LibraryElement::~LibraryElement()
{

}

void LibraryElement::setId(QString id)
{
    if (id.isEmpty())
       throw IllegalArgumentException("Element id cannot be empty");
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
        throw IllegalArgumentException("Element height must be positive");
    this->height = height;
}

void LibraryElement::setWidth(int width)
{
    if (width <= 0)
        throw IllegalArgumentException("Element width must be positive");
    this->width = width;
}