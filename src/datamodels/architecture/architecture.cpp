#include "architecture.h"

Architecture::Architecture(DistributionType type, AlgorithmIndexes indexes, AlgorithmParameters parameters)
{
    setDistributionType(type);
    setAlgorithmIndexes(indexes);
    setAlgorithmParameters(parameters);
}

void Architecture::setDistributionType(DistributionType type)
{
    this->type = type;
}

void Architecture::setAlgorithmIndexes(AlgorithmIndexes indexes)
{
    this->indexes = indexes;
}

void Architecture::setAlgorithmParameters(AlgorithmParameters parameters)
{
    this->parameters = parameters;
}

bool Architecture::operator ==(const Architecture& other) const
{
    if(model == other.model && type == other.type && indexes == other.indexes && parameters == other.parameters)
        return true;

    return false;
}
