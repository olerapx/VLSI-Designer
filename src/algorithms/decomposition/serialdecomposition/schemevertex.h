#pragma once

#include "datamodels/scheme/scheme.h"

/**
 * @brief The SchemeVertex class
 * Represents a scheme element with list of elements connected to it.
 */
class SchemeVertex
{
public:
    SchemeVertex(SchemeElement* element);

    SchemeElement* getElement() const { return element; }
    QList<std::pair<SchemeVertex*, WireType>>& getConnectedElements() { return connectedElements; }
    bool isDistributed() const { return distributed; }

    void setDistributed(bool distributed);
    bool operator ==(SchemeVertex* other);

private:
    SchemeElement* element;
    QList<std::pair<SchemeVertex*, WireType>> connectedElements;
    bool distributed;
};
