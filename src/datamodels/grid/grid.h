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
    QList <qint64> routedWires;
    int initialLevel;

public:
    Grid(int initialLevel);
    ~Grid();

    void setInitialLevel(int initialLevel);

    QList<QList<Cell*>>& getCells() {return this->cells;}
    QList <qint64>& getRoutedWires() {return this->routedWires;}
    int getInitialLevel() {return this->initialLevel;}
};

#endif // GRID_H
