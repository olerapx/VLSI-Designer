#include "statisticsentry.h"

StatisticsEntry::StatisticsEntry() :
    primaryPlacementTime(0),
    secondaryPlacementTime(0),
    innerRoutingTime(0),
    decompositionTime(0),
    compositionTime(0),
    outerRoutingTime(0),
    innerWiresNumber(0),
    wiresNumber(0),
    routedWiresNumber(0)
{

}

bool StatisticsEntry::operator ==(StatisticsEntry& other)
{
    if(hostName == other.hostName &&
            primaryPlacementTime == other.primaryPlacementTime && secondaryPlacementTime == other.secondaryPlacementTime &&
            innerRoutingTime == other.innerRoutingTime && decompositionTime == other.decompositionTime &&
            compositionTime == other.compositionTime && outerRoutingTime == other.outerRoutingTime &&
            innerWiresNumber == other.innerWiresNumber && wiresNumber == other.wiresNumber &&
            routedWiresNumber == other.routedWiresNumber)
        return true;

    return false;
}
