#ifndef SCHEME_H
#define SCHEME_H

#include <QList>

#include "schemeelement.h"
#include "wire.h"

/**
 * @brief The Scheme class
 * Represents a generated scheme.
 */
class Scheme: public Serializable
{
protected:
    QList<SchemeElement> elements;
    QList<Wire> wires;

public:
    Scheme();
    virtual ~Scheme();

    QList<SchemeElement>& getElements() { return elements; }
    QList<Wire>& getWires() { return wires; }
};

#endif // SCHEME_H
