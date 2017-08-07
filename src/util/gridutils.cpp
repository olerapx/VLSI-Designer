#include "gridutils.h"

QList<QList<Cell>> GridUtils::cut(Grid *grid, QPoint topLeftCoord, int width, int height)
{
    if((topLeftCoord.x() + width) > grid->getCells()[0].size())
        throw IllegalArgumentException(QObject::tr("Cannot cut the area with the width %1 from the grid: the width is too big.")
                                       .arg(QString::number(width)));

    if((topLeftCoord.y() + height) > grid->getCells().size())
        throw IllegalArgumentException(QObject::tr("Cannot cut the area with the height %1 from the grid: the height is too big.")
                                       .arg(QString::number(height)));


    if(width < 0 || height < 0)
        throw IllegalArgumentException(QObject::tr("Width and height cannot be negative."));

    QList<QList<Cell>> res;

    for(int i=topLeftCoord.y(); i<topLeftCoord.y() + height; i++)
    {
        QList<Cell> row;

        for(int j=topLeftCoord.x(); j<topLeftCoord.x() + width; j++)
        {
            row.append(grid->getCells()[i][j]);
            grid->getCells()[i][j] = Cell(CellType::Empty);
        }

        res.append(row);
    }

    return res;
}

void GridUtils::paste(Grid *grid, QList<QList<Cell>> data, QPoint topLeftCoord)
{
    int size = data.size();

    if(size == 0)
        return;

    if((topLeftCoord.y() + size) > grid->getCells().size())
        throw IllegalArgumentException(QObject::tr("Cannot paste the area with the height %1 from the grid: the height is too big.")
                                       .arg(QString::number(size)));

    for(int i=0; i<size; i++)
    {
        int rowSize = data[i].size();

        if((topLeftCoord.x() + rowSize) > grid->getCells()[i].size())
            throw IllegalArgumentException(QObject::tr("Cannot paste the area with the width %1 from the grid: the width is too big.")
                                           .arg(QString::number(rowSize)));

        for(int j=0; j<rowSize; j++)
        {
            grid->getCells()[topLeftCoord.y() + i][topLeftCoord.x() + j] = data[i][j];
        }
    }
}

void GridUtils::insertEmptyArea(Grid *grid, QPoint topLeftCoord, int width, int height)
{
    if(topLeftCoord.y() >= grid->getCells().size() || topLeftCoord.x() >= grid->getCells()[0].size())
        throw IllegalArgumentException(QObject::tr("Start point is out of range."));

    if(width < 0 || height < 0)
        throw IllegalArgumentException(QObject::tr("Width and height cannot be negative."));

    for(int i=topLeftCoord.y(); i<topLeftCoord.y() + height; i++)
    {
        for(int j=topLeftCoord.x(); j<topLeftCoord.x() + width; j++)
        {
            grid->getCells()[i].insert(j, Cell(CellType::Empty));
        }
    }
}

void GridUtils::removeArea(Grid *grid, QPoint topLeftCoord, int width, int height)
{
    if(topLeftCoord.y() >= grid->getCells().size() || topLeftCoord.x() >= grid->getCells()[0].size())
        throw IllegalArgumentException(QObject::tr("Start point is out of range."));

    if(width < 0 || height < 0)
        throw IllegalArgumentException(QObject::tr("Width and height cannot be negative."));

    for(int i=topLeftCoord.y(); i<topLeftCoord.y() + height; i++)
    {
        for(int j=0; j<width; j++)
        {
            grid->getCells()[i].removeAt(topLeftCoord.x());
        }
    }
}
