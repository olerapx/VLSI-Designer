#pragma once

#include <QList>

#include "statisticsentry.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The Statistics class
 * Represents the total statistics data split by distribution levels.
 */
class Statistics : public Serializable
{
public:
    Statistics(int topLevel);

    int getTopLevel() const { return topLevel; }
    QList<QList<StatisticsEntry>>& getData() { return data; }

    void merge(Statistics& other);

    bool operator ==(Statistics& other);

    double getAveragePrimaryPlacementTime(int level);
    double getAverageSecondaryPlacementTime(int level);
    double getAverageInternalRoutingTime(int level);

    double getAverageTotalInternalTime(int level);
    int getMaxTotalInternalTime(int level);

    double getAverageDecompositionTime(int level);
    double getAverageCompositionTime(int level);
    double getAverageExternalRoutingTime(int level);

    double getAverageTotalExternalTime(int level);
    int getMaxTotalExternalTime(int level);

    double getAverageRoutedWiresDelta(int level);
    double getAverageRoutedInternalWiresPercent(int level);
    double getAverageRoutedTotalWiresPercent(int level);

    int getTotalRoutedWiresDelta();

private:
    int topLevel;
    QList<QList<StatisticsEntry>> data;
};
