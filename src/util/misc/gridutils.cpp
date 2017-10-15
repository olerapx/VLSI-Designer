#include "gridutils.h"

QList<QList<Cell>> GridUtils::cut(Grid* grid, QPoint topLeftCoord, int width, int height)
{
    if((topLeftCoord.x() + width) > grid->getWidth())
        throw IllegalArgumentException(QObject::tr("Cannot cut the area with the width %1 from the grid: the width is too big.")
                                       .arg(QString::number(width)));

    if((topLeftCoord.y() + height) > grid->getHeight())
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

void GridUtils::paste(Grid* grid, QList<QList<Cell>> data, QPoint topLeftCoord)
{
    int size = data.size();

    if(size == 0)
        return;

    if((topLeftCoord.y() + size) > grid->getHeight())
        throw IllegalArgumentException(QObject::tr("Cannot paste the area with the height %1 into the grid: the height is too big.")
                                       .arg(QString::number(size)));

    for(int i=0; i<size; i++)
    {
        int rowSize = data[i].size();

        if((topLeftCoord.x() + rowSize) > grid->getWidth())
            throw IllegalArgumentException(QObject::tr("Cannot paste the area with the width %1 into the grid: the width is too big.")
                                           .arg(QString::number(rowSize)));

        for(int j=0; j<rowSize; j++)
        {
            grid->getCells()[topLeftCoord.y() + i][topLeftCoord.x() + j] = data[i][j];
        }
    }
}

void GridUtils::insertRows(Grid* grid, int y, int number)
{
    if(y > grid->getHeight())
        throw IllegalArgumentException(QObject::tr("Cannot insert rows after y-coordinate %1 at the grid: the number is too big.")
                                       .arg(QString::number(number)));

    int width = grid->getWidth();

    for(int i=0; i<number; i++)
    {
        QList<Cell> row;
        for(int j=0; j<width; j++)
            row.append(Cell(CellType::Empty));

        grid->getCells().insert(y, row);
    }
}

void GridUtils::removeRows(Grid* grid, int y, int number)
{
    if((y + number) > grid->getHeight())
        throw IllegalArgumentException(QObject::tr("Cannot remove %1 rows from the grid: the number is too big.")
                                       .arg(QString::number(number)));

    for(int i=0; i< number; i++)
        grid->getCells().removeAt(y);
}

void GridUtils::insertEmptyArea(Grid* grid, QPoint topLeftCoord, int colsNumber, int rowsNumber)
{
    if(topLeftCoord.y() >= grid->getHeight() || topLeftCoord.x() > grid->getWidth())
        throw IllegalArgumentException(QObject::tr("Start point is out of range."));

    if(colsNumber < 0 || rowsNumber < 0)
        throw IllegalArgumentException(QObject::tr("Width and height cannot be negative."));  

    for(int i=topLeftCoord.y(); i<topLeftCoord.y() + rowsNumber; i++)
    {
        for(int j=topLeftCoord.x(); j<topLeftCoord.x() + colsNumber; j++)
        {
            grid->getCells()[i].insert(j, Cell(CellType::Empty));
        }
    }
}

void GridUtils::removeArea(Grid* grid, QPoint topLeftCoord, int colsNumber, int rowsNumber)
{
    if(topLeftCoord.y() >= grid->getHeight() || topLeftCoord.x() >= grid->getWidth())
        throw IllegalArgumentException(QObject::tr("Start point is out of range."));

    if(colsNumber < 0 || rowsNumber < 0)
        throw IllegalArgumentException(QObject::tr("Width and height cannot be negative."));

    for(int i=topLeftCoord.y(); i<topLeftCoord.y() + rowsNumber; i++)
    {
        for(int j=0; j<colsNumber; j++)
        {
            grid->getCells()[i].removeAt(topLeftCoord.x());
        }
    }
}

Grid* GridUtils::rotate90(Grid* grid)
{
    Grid* res = new Grid();

    for(int i=0; i<grid->getWidth(); i++)
    {
        QList<Cell> row;

        for(int j=grid->getHeight() - 1; j>=0; j--)
        {
            Cell cell = grid->getCells()[j][i];
            cell.setType(rotateCellType(cell.getType()), cell.getIndex(), cell.getPinId());

            row.append(cell);
        }
        res->getCells().append(row);
    }

    res->getRoutedWires().append(grid->getRoutedWires());
    for(WireData& data: grid->getWiresData())
    {
        WireData newData = data;

        newData.setSrcCoord(QPoint(grid->getHeight() - 1 - data.getSrcCoord().y(), data.getSrcCoord().x()));

        if(data.getWirePosition() != WirePosition::External)
            newData.setDestCoord(QPoint(grid->getHeight() - 1 - data.getDestCoord().y(), data.getDestCoord().x()));

        res->getWiresData().append(newData);
    }

    return res;
}

CellType GridUtils::rotateCellType(CellType type)
{
    switch(type)
    {
    case CellType::UD:
        return CellType::LR;
    case CellType::LR:
        return CellType::UD;
    case CellType::UL:
        return CellType::UR;
    case CellType::UR:
        return CellType::DR;
    case CellType::DL:
        return CellType::UL;
    case CellType::DR:
        return CellType::DL;
    case CellType::UDL:
        return CellType::LRU;
    case CellType::UDR:
        return CellType::LRD;
    case CellType::LRU:
        return CellType::UDR;
    case CellType::LRD:
        return CellType::UDL;
    default:
        return type;
    }
}
