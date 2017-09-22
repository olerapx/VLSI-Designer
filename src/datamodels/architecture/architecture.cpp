#include "architecture.h"

Architecture::Architecture(DistributionType type, AlgorithmIndexes indexes)
{
    setDistributionType(type);
    setAlgorithmIndexes(indexes);
}

void Architecture::setDistributionType(DistributionType type)
{
    this->type = type;
}

void Architecture::setAlgorithmIndexes(AlgorithmIndexes indexes)
{
    this->indexes = indexes;
}

bool Architecture::operator ==(const Architecture& other)
{
    if(model == other.model && type == other.type)
        return true;

    return false;
}
