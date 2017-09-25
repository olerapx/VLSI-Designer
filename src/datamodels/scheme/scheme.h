#pragma once

#include <QList>

#include "datamodels/version/version.h"
#include "schemeelement.h"
#include "wire.h"

/**
 * @brief The Scheme class
 * Represents a generated scheme.
 */
class Scheme : public Serializable
{
protected:
    QList<QPair<QString, Version>> usedLibraries;

    QList<SchemeElement> elements;
    QList<Wire> wires;    

public:
    Scheme() {}
    virtual ~Scheme() {}

    QList<SchemeElement>& getElements() { return elements; }
    QList<Wire>& getWires() { return wires; }

    QList<QPair<QString, Version>>& getUsedLibraries() { return usedLibraries; }

    bool operator ==(const Scheme& other) const;
};
