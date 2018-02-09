#include "statistics.h"

Statistics::Statistics(int topLevel) :
    topLevel(topLevel)
{

}

void Statistics::merge(Statistics& other)
{
    if(other.topLevel < topLevel)
        throw IllegalArgumentException(QObject::tr("Cannot merge statistics from level %1 to level %2.")
                                       .arg(QString::number(other.getTopLevel()), QString::number(topLevel)));

    int maxLevel = topLevel + data.size() - 1;
    int otherMaxLevel = other.topLevel + other.data.size() - 1;

    for(int i = maxLevel; i<otherMaxLevel; i++)
        data.append(QList<StatisticsEntry>());

    for(int i=0; i<other.data.size(); i++)
    {
        data[other.topLevel - topLevel + i].append(other.data[i]);
    }
}

bool Statistics::operator ==(Statistics& other)
{
    if(topLevel == other.topLevel && data == other.data)
        return true;

    return false;
}

double Statistics::getAveragePrimaryPlacementTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getPrimaryPlacememtTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageSecondaryPlacementTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getSecondaryPlacementTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageInternalRoutingTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getInternalRoutingTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageTotalInternalTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getTotalInternalTime();

    return (((double) summaryTime) / data[level].size());
}

int Statistics::getMaxTotalInternalTime(int level)
{
    int maxTime = 0;
    for(StatisticsEntry& entry: data[level])
    {
        int totalTime = entry.getTotalInternalTime();
        if(totalTime > maxTime)
            maxTime = totalTime;
    }

    return maxTime;
}

double Statistics::getAverageDecompositionTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getDecompositionTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageCompositionTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getCompositionTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageExternalRoutingTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getExternalRoutingTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageTotalExternalTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getTotalExternalTime();

    return (((double) summaryTime) / data[level].size());
}

int Statistics::getMaxTotalExternalTime(int level)
{
    int maxTime = 0;
    for(StatisticsEntry& entry: data[level])
    {
        int totalTime = entry.getTotalExternalTime();
        if(totalTime > maxTime)
            maxTime = totalTime;
    }

    return maxTime;
}

double Statistics::getAverageRoutedWiresDelta(int level)
{
    double summaryPercent = 0;
    for(StatisticsEntry& entry: data[level])
        summaryPercent += entry.getRoutedWiresDelta();

    return (summaryPercent / data[level].size());
}

double Statistics::getAverageRoutedInternalWiresPercent(int level)
{
    double summaryPercent = 0;
    for(StatisticsEntry& entry: data[level])
        summaryPercent += entry.getRoutedInternalWiresPercent();

    return (summaryPercent / data[level].size());
}

double Statistics::getAverageRoutedTotalWiresPercent(int level)
{
    double summaryPercent = 0;
    for(StatisticsEntry& entry: data[level])
        summaryPercent += entry.getRoutedTotalWiresPercent();

    return (summaryPercent / data[level].size());
}

int Statistics::getTotalRoutedWiresDelta()
{
    int total = 0;

    for(QList<StatisticsEntry>& list: data)
    {
        for(StatisticsEntry entry: list)
        {
            total += entry.getRoutedWiresDelta();
        }
    }

    return total;
}
