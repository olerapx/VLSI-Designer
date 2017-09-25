#pragma once

#include "libraryelement.h"
#include "datamodels/version/version.h"

/**
 * @brief The Library class
 * Represents a library of elements.
 */
class Library : public Serializable
{
protected:
    QString id;
    Version version;
    QString name;
    QList<LibraryElement> elements;

public:
    Library(QString id, Version version);
    virtual ~Library() {}

    void setId(QString id);
    void setVersion(Version version);
    void setName(QString name);

    QString getId() const { return id; }
    Version getVersion() const { return version; }
    QString getName() const { return name; }
    QList<LibraryElement>& getElements() { return elements; }

    bool operator ==(const Library& other) const;
};
