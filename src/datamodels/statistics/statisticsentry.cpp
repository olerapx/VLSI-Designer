#include "statisticsentry.h"

StatisticsEntry::StatisticsEntry() :
    primaryPlacementTime(0),
    secondaryPlacementTime(0),
    internalRoutingTime(0),
    decompositionTime(0),
    compositionTime(0),
    externalRoutingTime(0),
    internalUnroutedWiresNumber(0),
    totalUnroutedWiresNumber(0),
    routedWiresDelta(0)
{

}

bool StatisticsEntry::operator ==(StatisticsEntry& other)
{
    if(hostName == other.hostName &&
            primaryPlacementTime == other.primaryPlacementTime && secondaryPlacementTime == other.secondaryPlacementTime &&
            internalRoutingTime == other.internalRoutingTime && decompositionTime == other.decompositionTime &&
            compositionTime == other.compositionTime && externalRoutingTime == other.externalRoutingTime &&
            internalUnroutedWiresNumber == other.internalUnroutedWiresNumber &&
            totalUnroutedWiresNumber == other.totalUnroutedWiresNumber &&
            routedWiresDelta == other.routedWiresDelta)
        return true;

    return false;
}
