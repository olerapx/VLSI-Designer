#include "library.h"

Library::Library(QString id, Version version)
{
    setId(id);
    setVersion(version);

    name = "";
}

void Library::setId(QString id)
{
    if (id.isEmpty())
        throw IllegalArgumentException(QObject::tr("Library id cannot be empty."));
    this->id = id;
}

void Library::setVersion(Version version)
{
    this->version = version;
}

void Library::setName(QString name)
{
    this->name = name;
}

bool Library::operator ==(const Library& other) const
{
    if(id == other.id && version == other.version &&
       name == other.name && elements == other.elements)
        return true;

    return false;
}
