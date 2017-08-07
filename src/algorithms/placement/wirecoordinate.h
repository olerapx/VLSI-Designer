#ifndef WIRECOORDINATE_H
#define WIRECOORDINATE_H

#include "elementcoordinate.h"
#include "datamodels/scheme/wire.h"
#include "datamodels/library/library.h"
#include "util/libraryutils.h"

enum class WirePosition
{
    Internal,
    External
};

/**
 * @brief The WireCoordinate class
 * Encapsulates a wire with the coordinates of its pins and the fitness function value.
 */
class WireCoordinate
{
public:
    WireCoordinate(Wire& wire, QList<ElementCoordinate>& elementCoordinates, QList<Library*>& libraries);

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

    QPoint getPinCoord(ElementCoordinate* coordinate, QList<Library*>& libraries, QString pinId);
};

#endif // WIRECOORDINATE_H
