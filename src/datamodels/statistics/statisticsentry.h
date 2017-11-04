#pragma once

#include <QString>

#include "datamodels/serializable.h"

/**
 * @brief The StatisticsEntry class
 * Encapsulates the design process statistics on a signle node.
 */
class StatisticsEntry : public Serializable
{
public:
    StatisticsEntry();

    QString getHostName() const { return hostName; }

    int getPrimaryPlacememtTime() const { return primaryPlacementTime; }
    int getSecondaryPlacementTime() const { return secondaryPlacementTime; }
    int getInternalRoutingTime() const { return internalRoutingTime; }
    int getTotalInternalTime() const { return primaryPlacementTime + secondaryPlacementTime + internalRoutingTime; }

    int getDecompositionTime() const { return decompositionTime; }
    int getCompositionTime() const { return compositionTime; }
    int getExternalRoutingTime() const { return externalRoutingTime; }

    int getTotalExternalTime() const { return decompositionTime + compositionTime + externalRoutingTime; }

    int getInternalUnroutedWiresNumber() const { return internalUnroutedWiresNumber; }
    int getTotalUnroutedWiresNumber() const { return totalUnroutedWiresNumber; }

    int getRoutedWiresDelta() const { return routedWiresDelta; }
    double getRoutedInternalWiresPercent() const { return (((double)routedWiresDelta * 100) / internalUnroutedWiresNumber); }
    double getRoutedTotalWiresPercent() const { return (((double)routedWiresDelta * 100) / totalUnroutedWiresNumber); }

    void setHostName(QString name) { hostName = name; }

    void setPrimaryPlacementTime(int time) { primaryPlacementTime = time; }
    void setSecondaryPlacementTime(int time) { secondaryPlacementTime = time; }
    void setInternalRoutingTime(int time) { internalRoutingTime = time; }

    void setDecompositionTime(int time) { decompositionTime = time; }
    void setCompositionTime(int time) { compositionTime = time; }
    void setExternalRoutingTime(int time) { externalRoutingTime = time; }

    void setInternalUnroutedWiresNumber(int number) { internalUnroutedWiresNumber = number; }
    void setTotalUnroutedWiresNumber(int number) { totalUnroutedWiresNumber = number; }

    void setRoutedWiresDelta(int delta) { routedWiresDelta = delta; }

    bool operator ==(StatisticsEntry& other);

private:
    QString hostName;

    int primaryPlacementTime;
    int secondaryPlacementTime;
    int internalRoutingTime;

    int decompositionTime;
    int compositionTime;
    int externalRoutingTime;

    int internalUnroutedWiresNumber;
    int totalUnroutedWiresNumber;
    int routedWiresDelta;
};
