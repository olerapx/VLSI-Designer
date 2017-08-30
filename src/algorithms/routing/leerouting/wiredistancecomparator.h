#ifndef WIREDISTANCECOMPARATOR_H
#define WIREDISTANCECOMPARATOR_H

#include "datamodels/grid/wiredata.h"
#include "util/misc/wireutils.h"

class WireDistanceComparator
{
public:
    WireDistanceComparator(int gridHeight, int gridWidth);

    bool operator ()(WireData* first, WireData* second);

private:
    int height, width;
};

#endif // WIREDISTANCECOMPARATOR_H
