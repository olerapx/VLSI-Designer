#ifndef GRID_H
#define GRID_H

#include <QList>
#include "cell.h"
#include "routedwireindex.h"

/**
 * @brief The Grid class
 */
class Grid: public Serializable
{
protected:
    QList<QList<Cell>> cells;
    QList <RoutedWireIndex> routedWires;
    int initialLevel; /// Client's initial level.

public:
    Grid(int initialLevel);
    virtual ~Grid() {}

    void setInitialLevel(int initialLevel);

    QList<QList<Cell>>& getCells() { return cells; }
    QList <RoutedWireIndex>& getRoutedWires() { return routedWires; }
    int getInitialLevel() const { return initialLevel; }

    bool operator ==(const Grid& other);
};

#endif // GRID_H
