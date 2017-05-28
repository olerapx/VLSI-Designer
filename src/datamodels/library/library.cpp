#include "library.h"

Library::Library(QString id, double version)
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

void Library::setVersion(double version)
{
    if (version < 0.0)
        throw IllegalArgumentException (QObject::tr("Library version cannot be negative, passed: %1.")
                                        .arg(QString::number(version)));
    this->version = version;
}

void Library::setName(QString name)
{
    this->name = name;
}

bool Library::operator ==(const Library& other)
{
    if(id == other.id && version == other.version &&
            name == other.name && elements == other.elements)
        return true;

    return false;
}
