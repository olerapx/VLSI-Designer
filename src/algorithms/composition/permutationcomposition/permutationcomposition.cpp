#include "permutationcomposition.h"

PermutationComposition::PermutationComposition(QList<Grid*> grids) :
    CompositionAlgorithm(grids)
{
    clear();
}

Grid* PermutationComposition::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        stretchParts();
        fillPositions();

        Grid* grid = buildResult();

        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

        stopped = true;
        actuallyStopped = true;

        return grid;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void PermutationComposition::clear()
{
    maxSize = QSize(0, 0);
    positions.clear();

    gridHeight = 0;
    gridWidth = 0;
}

void PermutationComposition::stretchParts()
{
    getMaxGridSize();

    for(Grid* grid: grids)
    {
        int diffHeight = maxSize.height() - grid->getCells().size();
        int diffWidth = maxSize.width() - grid->getCells()[0].size();

        stretchHeight(grid, diffHeight);
        stretchWidth(grid, diffWidth);
    }
}

void PermutationComposition::getMaxGridSize()
{
    for(Grid* grid: grids)
    {
        if(grid->getCells().size() > maxSize.height())
            maxSize.setHeight(grid->getCells().size());

        if(grid->getCells()[0].size() > maxSize.width())
            maxSize.setWidth(grid->getCells()[0].size());
    }
}

void PermutationComposition::stretchHeight(Grid* grid, int number)
{
    int upNumber = number / 2;
    int downNumber = number - upNumber;

    GridUtils::insertRows(grid, 0, upNumber);
    GridUtils::insertRows(grid, grid->getCells().size(), downNumber);

    for(WireData& data: grid->getWiresData())
    {
        data.setSrcCoord(data.getSrcCoord() + QPoint(0, upNumber));
        data.setDestCoord(data.getDestCoord() + QPoint(0, upNumber));
    }
}

void PermutationComposition::stretchWidth(Grid* grid, int number)
{
    int leftNumber = number / 2;
    int rightNumber = number - leftNumber;

    GridUtils::insertEmptyArea(grid, QPoint(0, 0), leftNumber, grid->getCells().size());
    GridUtils::insertEmptyArea(grid, QPoint(grid->getCells()[0].size(), 0), rightNumber, grid->getCells().size());

    for(WireData& data: grid->getWiresData())
    {
        data.setSrcCoord(data.getSrcCoord() + QPoint(leftNumber, 0));
        data.setDestCoord(data.getDestCoord() + QPoint(rightNumber, 0));
    }
}

void PermutationComposition::fillPositions()
{
    getGridWidth();

    QPoint nextPosition(0, 0);

    for(int i=0; i<grids.size(); i++)
    {
        positions.append({ nextPosition, i });
        nextPosition += QPoint(maxSize.width(), 0);

        if(nextPosition.x() + maxSize.width() > gridWidth)
            nextPosition = QPoint(0, nextPosition.y() + maxSize.height());
    }

    gridHeight = positions.last().coord.y() + maxSize.height();

    centerLastRow();
}

void PermutationComposition::getGridWidth()
{
   int totalSquare = maxSize.height() * maxSize.width() * grids.size();

   gridWidth = floor(sqrt(totalSquare));
   if(gridWidth * (gridWidth + 1) >= totalSquare)
       return;

   gridWidth ++;
}

void PermutationComposition::centerLastRow()
{
    int lastRowY = 0;
    int lastRowIndex = 0;

    for(int i=0; i<positions.size(); i++)
    {
        if(positions[i].coord.y() != lastRowY)
        {
            lastRowY = positions[i].coord.y();
            lastRowIndex = i;
        }
    }

    int lastRowWidth = (positions.size() - lastRowIndex) * maxSize.width();

    if(lastRowWidth == gridWidth)
        return;

    int diff = gridWidth - lastRowWidth;

    for(int i=lastRowIndex; i<positions.size(); i++)
        positions[i].coord = positions[i].coord + QPoint(diff/2, 0);
}

Grid* PermutationComposition::buildResult()
{
    Grid* grid = createEmptyTotalGrid();

    for(GridPosition& position: positions)
    {
        QPoint coord = position.coord;
        Grid* sourceGrid = grids[position.gridIndex];

        for(int i=0; i<maxSize.height(); i++)
        {
            for(int j=0; j<maxSize.width(); j++)
            {
                grid->getCells()[coord.y() + i][coord.x() + j] = sourceGrid->getCells()[i][j];
            }
        }
    }

    // ADD WIRESDATA

    return grid;
}

Grid* PermutationComposition::createEmptyTotalGrid()
{
    Grid* grid = new Grid();

    for(int i=0; i<gridHeight; i++)
    {
        grid->getCells().append(QList<Cell>());

        for(int j=0; j<gridWidth; j++)
        {
            grid->getCells().last().append(Cell(CellType::Empty));
        }
    }

    return grid;
}
