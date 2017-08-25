#include "pointdistancecomparator.h"

PointDistanceComparator::PointDistanceComparator(QPoint finish)
{
    this->finish = finish;
}

bool PointDistanceComparator::operator ()(QPoint& first, QPoint& second)
{
    return (WireUtils::getDistance(first, finish, WirePosition::Internal, 0, 0) <
            WireUtils::getDistance(second, finish, WirePosition::Internal, 0, 0));
}
