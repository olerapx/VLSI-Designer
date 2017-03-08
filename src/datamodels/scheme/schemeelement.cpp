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
        throw IllegalArgumentException ("Library id cannot be empty");
    this->libraryId = libraryId;
}

void SchemeElement::setElementId(QString elementId)
{
    if (elementId.isEmpty())
        throw IllegalArgumentException ("Element id cannot be empty");
    this->elementId = elementId;
}

void SchemeElement::setIndex(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException ("Element index cannot be negative");
    this->index = index;
}
