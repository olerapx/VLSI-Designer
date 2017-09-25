#pragma once

#include "datamodels/grid/wiredata.h"
#include "util/misc/wireutils.h"

class WireDistanceComparator
{
public:
    WireDistanceComparator(int gridHeight, int gridWidth);

    bool operator ()(WireData* first, WireData* second) const;

private:
    int height, width;
};
