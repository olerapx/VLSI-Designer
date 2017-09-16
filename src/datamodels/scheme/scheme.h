#pragma once

#include <QList>

#include "schemeelement.h"
#include "wire.h"

/**
 * @brief The Scheme class
 * Represents a generated scheme.
 */
class Scheme : public Serializable
{
protected:
    QList<QPair<QString, double>> usedLibraries;

    QList<SchemeElement> elements;
    QList<Wire> wires;    

public:
    Scheme() {}
    virtual ~Scheme() {}

    QList<SchemeElement>& getElements() { return elements; }
    QList<Wire>& getWires() { return wires; }

    QList<QPair<QString, double>>& getUsedLibraries() { return usedLibraries; }

    bool operator ==(const Scheme& other);
};
