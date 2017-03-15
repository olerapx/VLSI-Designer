#ifndef GRID_H
#define GRID_H

#include <QList>
#include "cell.h"

/**
 * @brief The Grid class
 * Note that an instance of this class will delete all contained cells in destructor.
 */
class Grid: public Serializable
{
protected:
    QList< QList<Cell*> > cells;
    QList <qint64> routedWires; /// Routed wires' indexes.
    int initialLevel; /// Client's initial level.

public:
    Grid(int initialLevel);
    virtual ~Grid();

    void setInitialLevel(int initialLevel);

    QList< QList<Cell*> >& getCells() {return cells;}
    QList <qint64>& getRoutedWires() {return routedWires;}
    int getInitialLevel() {return initialLevel;}
};

#endif // GRID_H
