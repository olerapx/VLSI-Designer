#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QList>

#include "datamodels/serializable.h"
#include "exception/illegalargumentexception.h"

enum class DistributionType
{
    Default,
    Greedy
};

/**
 * @brief The Architecture class
 * Represents the used architecture.
 */
class Architecture: public Serializable
{
protected:
    QList<int> model; /// Array of each server's clients number on each level start from 1.
    DistributionType type; /// Type of distribution algorithm.

public:
    Architecture(DistributionType type);

    void setDistributionType (DistributionType type);

    QList<int>& getModel() {return model;}
    DistributionType getDistributionType() {return type;}
};

#endif // ARCHITECTURE_H
