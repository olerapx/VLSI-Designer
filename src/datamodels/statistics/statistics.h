#pragma once

#include <QList>

#include "statisticsentry.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The Statistics class
 * Represents the total statistics data split by distribution levels.
 */
class Statistics
{
public:
    Statistics(int topLevel);

    int getTopLevel() const { return topLevel; }
    QList<QList<StatisticsEntry>> getData() { return data; }

    void merge(Statistics& other);

private:
    int topLevel;

    QList<QList<StatisticsEntry>> data;
};
