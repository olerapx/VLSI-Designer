#pragma once

#include "datamodels/grid/wiredata.h"
#include "datamodels/scheme/wire.h"

class WireUtils
{
public:
    static qint64 getDistance(QPoint src, QPoint dest, WirePosition position, int gridHeight, int gridWidth);
};
