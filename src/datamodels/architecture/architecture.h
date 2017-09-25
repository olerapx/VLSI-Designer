#pragma once

#include <QList>

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"
#include "clientsnumber.h"
#include "algorithmindexes.h"
#include "algorithmparameters.h"

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
    AlgorithmParameters parameters;

public:
    Architecture(DistributionType type, AlgorithmIndexes indexes, AlgorithmParameters parameters);
    virtual ~Architecture() {}

    void setDistributionType(DistributionType type);
    void setAlgorithmIndexes(AlgorithmIndexes indexes);
    void setAlgorithmParameters(AlgorithmParameters parameters);

    QList<ClientsNumber>& getModel() { return model; }
    DistributionType getDistributionType() const { return type; }
    AlgorithmIndexes getAlgorithmIndexes() { return indexes; }
    AlgorithmParameters getAlgorithmParameters() { return parameters; }

    bool operator ==(const Architecture& other) const;
};
