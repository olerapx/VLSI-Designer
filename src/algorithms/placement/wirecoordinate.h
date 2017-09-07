#pragma once

#include "elementcoordinate.h"
#include "datamodels/scheme/wire.h"
#include "datamodels/library/library.h"
#include "datamodels/grid/grid.h"
#include "util/misc/libraryutils.h"
#include "util/misc/wireutils.h"

/**
 * @brief The WireCoordinate class
 * Encapsulates a wire with the coordinates of its pins and the fitness function value.
 */
class WireCoordinate
{
public:
    WireCoordinate(Wire& wire, QList<QList<ElementCoordinate>>& elementCoordinates, QList<Library*>& libraries, int gridHeight, int gridWidth);

    Wire* getWire() { return wire; }

    QPoint getSrcCoordinate();
    QPoint getDestCoordinate();

    WirePosition getPosition() { return position; }

    qint64 getFitnessValue();

    bool operator ==(WireCoordinate& other);

private:
    static const int innerWireFitnessCoefficient = 10;

    Wire* wire;
    ElementCoordinate *src, *dest;
    WirePosition position;
    QPoint srcCoord, destCoord;

    int height, width;

    QPoint getPinCoord(ElementCoordinate* coordinate, QList<Library*>& libraries, QString pinId);
};
