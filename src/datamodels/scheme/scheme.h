#ifndef SCHEME_H
#define SCHEME_H

#include <QList>

#include "schemeelement.h"
#include "wire.h"

/**
 * @brief The Scheme class
 * Represents a generated scheme.
 * Note that an instance of this class will delete all contained elements and wires in destructor.
 */
class Scheme: public Serializable
{
protected:
    QList<SchemeElement*> elements;
    QList<Wire*> wires;

public:
    Scheme();
    ~Scheme();

    QList<SchemeElement*>& getElements() {return elements;}
    QList<Wire*>& getWires() {return wires;}
};

#endif // SCHEME_H
