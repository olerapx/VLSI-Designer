#ifndef LIBRARY_H
#define LIBRARY_H

#include <QList>
#include "libraryelement.h"

/**
 * @brief The Library class
 * Represents a library of elements.
 */
class Library: public Serializable
{
protected:
    QString id;
    double version;
    QString name;
    QList<LibraryElement> elements;

public:
    Library(QString id, double version);
    virtual ~Library();

    void setId(QString id);
    void setVersion(double version);
    void setName(QString name);

    QString getId() const { return id; }
    double getVersion() const { return version; }
    QString getName() const { return name; }
    QList<LibraryElement>& getElements() { return elements; }
};

#endif // LIBRARY_H
