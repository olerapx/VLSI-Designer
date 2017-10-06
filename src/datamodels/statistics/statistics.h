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
    double getAverageInnerRoutingTime(int level);

    double getAverageTotalInnerTime(int level);

    double getAverageDecompositionTime(int level);
    double getAverageCompositionTime(int level);
    double getAverageOuterRoutingTime(int level);

    double getAverageTotalOuterTime(int level);

    double getAverageInnerRoutedWiresPercent(int level);
    double getAverageRoutedWiresPercent(int level);

private:
    int topLevel;
    QList<QList<StatisticsEntry>> data;
};
