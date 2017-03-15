#ifndef SCHEMEELEMENT_H
#define SCHEMEELEMENT_H

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The SchemeElement class
 * Represents an element on a generated scheme.
 */
class SchemeElement: public Serializable
{
protected:
    QString libraryId;
    QString elementId;
    qint64 index;

public:
    SchemeElement(QString libraryId, QString elementId, qint64 index);
    virtual ~SchemeElement(){}

    void setLibraryId(QString libraryId);
    void setElementId(QString elementId);
    void setIndex(qint64 index);

    QString getLibraryId() {return libraryId;}
    QString getElementId() {return elementId;}
    qint64 getIndex() {return index;}
};

#endif // SCHEMEELEMENT_H
