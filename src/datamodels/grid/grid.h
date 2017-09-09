#pragma once

#include <QList>

#include "cell.h"
#include "routedwireindex.h"
#include "wiredata.h"

/**
 * @brief The Grid class
 */
class Grid : public Serializable
{
protected:
    QList<QList<Cell>> cells;
    QList <RoutedWireIndex> routedWires;
    QList<WireData> wiresData;

public:
    Grid() {}
    virtual ~Grid() {}

    QList<QList<Cell>>& getCells() { return cells; }
    QList <RoutedWireIndex>& getRoutedWires() { return routedWires; }
    QList<WireData>& getWiresData() { return wiresData; }

    int getHeight();
    int getWidth();

    bool operator ==(const Grid& other);
};
