#ifndef GRID_H
#define GRID_H

#include <QList>
#include "cell.h"
#include "routedwireindex.h"

/**
 * @brief The Grid class
 * Note that an instance of this class will delete all contained cells in destructor.
 */
class Grid: public Serializable
{
protected:
    QList< QList<Cell*> > cells;
    QList <RoutedWireIndex> routedWires; /// Routed wires' indexes.
    int initialLevel; /// Client's initial level.

public:
    Grid(int initialLevel);
    virtual ~Grid();

    void setInitialLevel(int initialLevel);

    QList< QList<Cell*> >& getCells() {return cells;}
    QList <RoutedWireIndex>& getRoutedWires() {return routedWires;}
    int getInitialLevel() const {return initialLevel;}
};

#endif // GRID_H
