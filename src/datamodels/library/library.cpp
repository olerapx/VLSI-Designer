#include "library.h"

Library::Library(QString id, double version)
{
    setId(id);
    setVersion(version);

    name = "";
}

Library::~Library()
{
    foreach (Element* element, elements)
        delete element;
    elements.clear();
}

void Library::setId(QString id)
{
    if (id.isEmpty())
        throw IllegalArgumentException("Library id cannot be empty");
    this->id = id;
}

void Library::setVersion(double version)
{
    if (version < 0.0)
        throw IllegalArgumentException ("Library version cannot be negative");
    this->version = version;
}

void Library::setName (QString name)
{
    this->name = name;
}
