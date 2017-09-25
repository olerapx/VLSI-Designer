#include "schemeelement.h"

SchemeElement::SchemeElement(QString libraryId, QString elementId, qint64 index)
{
    setLibraryId(libraryId);
    setElementId(elementId);
    setIndex(index);
}

void SchemeElement::setLibraryId(QString libraryId)
{
    if (libraryId.isEmpty())
        throw IllegalArgumentException (QObject::tr("Library id cannot be empty."));
    this->libraryId = libraryId;
}

void SchemeElement::setElementId(QString elementId)
{
    if (elementId.isEmpty())
        throw IllegalArgumentException (QObject::tr("Element id cannot be empty."));
    this->elementId = elementId;
}

void SchemeElement::setIndex(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException (QObject::tr("Element index cannot be negative, passed: %1.")
                                        .arg(QString::number(index)));
    this->index = index;
}

void SchemeElement::setAlias(QString alias)
{
    this->alias = alias;
}

bool SchemeElement::operator ==(const SchemeElement& other) const
{
    if(libraryId == other.libraryId && elementId == other.elementId)
        return true;

    return false;
}
