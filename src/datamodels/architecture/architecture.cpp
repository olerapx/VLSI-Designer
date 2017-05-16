#include "architecture.h"

Architecture::Architecture(DistributionType type)
{
    setDistributionType(type);
}

void Architecture::setDistributionType(DistributionType type)
{
    this->type = type;
}

bool Architecture::operator ==(const Architecture& other)
{
    if(model == other.model && type == other.type)
        return true;

    return false;
}
