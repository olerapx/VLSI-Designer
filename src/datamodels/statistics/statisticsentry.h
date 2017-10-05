#pragma once

/**
 * @brief The StatisticsEntry class
 * Encapsulates the design process statistics on a signle node.
 */
class StatisticsEntry
{
public:
    StatisticsEntry();

    int getPrimaryPlacememtTime() const { return primaryPlacementTime; }
    int getSecondaryPlacementTime() const { return secondaryPlacementTime; }
    int getInnerRoutingTime() const { return innerRoutingTime; }
    int getTotalInnerTime() const { return primaryPlacementTime + secondaryPlacementTime + innerRoutingTime; }

    int getDecompositionTime() const { return decompositionTime; }
    int getCompositionTime() const { return compositionTime; }
    int getOuterRoutingTime() const { return outerRoutingTime; }

    int getInnerWiresNumber() const { return innerWiresNumber; }
    int getTotalChildrenInnerWiresNumber() const;

    int getWiresNumber() const { return wiresNumber; }
    double getOuterWiresPercent() const { return (((double)(wiresNumber - innerWiresNumber)) / wiresNumber); }

    int getRoutedWiresNumber() const { return routedWiresNumber; }
    double getInnerRoutedWiresPercent() const { return (((double) routedWiresNumber) / innerWiresNumber); }
    double getRoutedWiresPercent() const { return (((double) routedWiresNumber) / wiresNumber); }

    void setPrimaryPlacementTime(int time) { primaryPlacementTime = time; }
    void setSecondaryPlacementTime(int time) { secondaryPlacementTime = time; }
    void setInnerRoutingTime(int time) { innerRoutingTime = time; }

    void setDecompositionTime(int time) { decompositionTime = time; }
    void setCompositionTime(int time) { compositionTime = time; }
    void setOuterRoutingTime(int time) { outerRoutingTime = time; }

    void setInnerWiresNumber(int number) { innerWiresNumber = number; }
    void setWiresNumber(int number) { wiresNumber = number; }

    void setRoutedWiresNumber(int number) { routedWiresNumber = number; }

private:
    int primaryPlacementTime;
    int secondaryPlacementTime;
    int innerRoutingTime;

    int decompositionTime;
    int compositionTime;
    int outerRoutingTime;

    int innerWiresNumber;
    int wiresNumber;

    int routedWiresNumber;
};
