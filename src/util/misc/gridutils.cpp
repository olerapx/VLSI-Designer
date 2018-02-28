#include "gridutils.h"

void GridUtils::removeRows(Grid* grid, int y, int number)
{
    if((y + number) > grid->getHeight())
        throw IllegalArgumentException(QObject::tr("Cannot remove %1 rows from the grid: the number is too big.")
                                       .arg(QString::number(number)));

    for(int i=0; i< number; i++)
        grid->getCells().removeAt(y);
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

void GridUtils::drawElement(Grid* grid, QPoint point, SchemeElement element, QList<Library*> libraries)
{
    LibraryElement libraryElement = LibraryUtils::getCorrespondingElement(element, libraries);

    for(int i=1; i<libraryElement.getHeight() - 1; i++)
    {
        for(int j=1; j<libraryElement.getWidth() - 1; j++)
        {
            grid->getCells()[point.y() + i][point.x() + j] = Cell(CellType::Element, element.getIndex());
        }
    }

    for(Pin p: libraryElement.getPins())
    {
        grid->getCells()[point.y() + p.getY()][point.x() + p.getX()] = Cell(CellType::Pin, element.getIndex(), p.getId());
    }
}
