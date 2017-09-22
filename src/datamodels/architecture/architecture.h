#pragma once

#include <QList>

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"
#include "clientsnumber.h"
#include "algorithmindexes.h"

/**
 * @brief The DistributionType enum
 * Represents the possible types of distribution.
 */
enum class DistributionType
{
    Default,
    Greedy
};

/**
 * @brief The Architecture class
 * Represents the used architecture.
 */
class Architecture : public Serializable
{
protected:
    QList<ClientsNumber> model; /// List of each server's clients number on each level start from 1.
    DistributionType type;
    AlgorithmIndexes indexes;

public:
    Architecture(DistributionType type, AlgorithmIndexes indexes);
    virtual ~Architecture() {}

    void setDistributionType(DistributionType type);
    void setAlgorithmIndexes(AlgorithmIndexes indexes);

    QList<ClientsNumber>& getModel() { return model; }
    DistributionType getDistributionType() const { return type; }
    AlgorithmIndexes getAlgorithmIndexes() { return indexes; }

    bool operator ==(const Architecture& other);
};
