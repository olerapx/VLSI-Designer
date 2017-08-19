#ifndef SCHEMEELEMENT_H
#define SCHEMEELEMENT_H

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The SchemeElement class
 * Represents an element on a generated scheme.
 */
class SchemeElement : public Serializable
{
protected:
    QString libraryId;
    QString elementId;
    qint64 index;
    QString alias;

public:
    SchemeElement(QString libraryId, QString elementId, qint64 index);
    virtual ~SchemeElement() {}

    void setLibraryId(QString libraryId);
    void setElementId(QString elementId);
    void setIndex(qint64 index);
    void setAlias(QString alias);

    QString getLibraryId() const { return libraryId; }
    QString getElementId() const { return elementId; }
    qint64 getIndex() const { return index; }
    QString getAlias() const { return alias; }

    /**
     * @brief operator ==
     * @param other
     * @return true if elements are equal regardless of their indexes, false otherwise.
     */
    bool operator ==(const SchemeElement& other);
};

#endif // SCHEMEELEMENT_H
