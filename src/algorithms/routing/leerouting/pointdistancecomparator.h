#ifndef POINTDISTANCECOMPARATOR_H
#define POINTDISTANCECOMPARATOR_H

#include <QPoint>

#include "util/misc/wireutils.h"

class PointDistanceComparator
{
public:
    PointDistanceComparator(QPoint finish);

    bool operator ()(QPoint& first, QPoint& second);

private:
    QPoint finish;
};

#endif // POINTDISTANCECOMPARATOR_H
