#include "wireutils.h"

qint64 WireUtils::getDistance(QPoint src, QPoint dest, WirePosition position, int gridHeight, int gridWidth)
{
    qint64 res = 0;

    if(position == WirePosition::External)
    {
        int distances[] = { src.x(), src.y(), gridHeight - src.y() - 1, gridWidth - src.x() - 1 };
        int min = distances[0];

        for(int i=1; i<4; i++)
            if(distances[i] < min)
                min = distances[i];

        res = min;
    }
    else
    {
        res = abs(src.x() - dest.x()) + abs(src.y() - dest.y()) - 1;
    }

    return res;
}
