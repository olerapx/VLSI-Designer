#pragma once

#include "util/misc/wireutils.h"

class PointDistanceComparator
{
public:
    PointDistanceComparator(QPoint finish);

    bool operator ()(QPoint& first, QPoint& second) const;

private:
    QPoint finish;
};
