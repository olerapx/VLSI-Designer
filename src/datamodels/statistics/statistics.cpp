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

double Statistics::getAverageInnerRoutingTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getInnerRoutingTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageTotalInnerTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getTotalInnerTime();

    return (((double) summaryTime) / data[level].size());
}

int Statistics::getMaxTotalInnerTime(int level)
{
    int maxTime = 0;
    for(StatisticsEntry& entry: data[level])
    {
        int totalTime = entry.getTotalInnerTime();
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

double Statistics::getAverageOuterRoutingTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getOuterRoutingTime();

    return (((double) summaryTime) / data[level].size());
}

double Statistics::getAverageTotalOuterTime(int level)
{
    int summaryTime = 0;
    for(StatisticsEntry& entry: data[level])
        summaryTime += entry.getTotalOuterTime();

    return (((double) summaryTime) / data[level].size());
}

int Statistics::getMaxTotalOuterTime(int level)
{
    int maxTime = 0;
    for(StatisticsEntry& entry: data[level])
    {
        int totalTime = entry.getTotalOuterTime();
        if(totalTime > maxTime)
            maxTime = totalTime;
    }

    return maxTime;
}

double Statistics::getAverageInnerRoutedWiresPercent(int level)
{
    double summaryPercent = 0;
    for(StatisticsEntry& entry: data[level])
        summaryPercent += entry.getInnerRoutedWiresPercent();

    return (summaryPercent / data[level].size());
}

double Statistics::getAverageRoutedWiresPercent(int level)
{
    double summaryPercent = 0;
    for(StatisticsEntry& entry: data[level])
        summaryPercent += entry.getRoutedWiresPercent();

    return (summaryPercent / data[level].size());
}
