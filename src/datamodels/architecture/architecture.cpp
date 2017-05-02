#include "architecture.h"

Architecture::Architecture(DistributionType type)
{
    setDistributionType(type);
}

void Architecture::setDistributionType(DistributionType type)
{
    this->type = type;
}
