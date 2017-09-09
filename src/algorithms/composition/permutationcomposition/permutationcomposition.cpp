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

        fillOffsets();
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
    offsets.clear();

    gridHeight = 0;
    gridWidth = 0;
}

void PermutationComposition::fillOffsets()
{
    getMaxGridSize();

    for(Grid* grid: grids)
    {
        int diffHeight = maxSize.height() - grid->getHeight();
        int diffWidth = maxSize.width() - grid->getWidth();

        offsets.append(QPoint(diffWidth / 2, diffHeight / 2));
    }
}

void PermutationComposition::getMaxGridSize()
{
    for(Grid* grid: grids)
    {
        if(grid->getHeight() > maxSize.height())
            maxSize.setHeight(grid->getHeight());

        if(grid->getWidth() > maxSize.width())
            maxSize.setWidth(grid->getWidth());
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

        QPoint offset = offsets[position.gridIndex];

        for(int i=0; i<sourceGrid->getHeight(); i++)
        {
            for(int j=0; j<sourceGrid->getWidth(); j++)
            {
                grid->getCells()[coord.y() + offset.y() + i][coord.x() + offset.x() + j] =
                        sourceGrid->getCells()[i][j];
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
