#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QList>

#include "src/datamodels/serializable.h"
#include "src/exception/illegalargumentexception.h"

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
    QList<int> model;
    DistributionType type;

public:
    Architecture(DistributionType type);

    void setDistributionType (DistributionType type);

    QList<int>& getModel() {return this->model;}
    DistributionType getDistributionType() {return this->type;}
};

#endif // ARCHITECTURE_H
