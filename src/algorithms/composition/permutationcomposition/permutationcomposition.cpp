#include "permutationcomposition.h"

PermutationComposition::PermutationComposition(QList<Grid*> grids, Scheme* scheme) :
    CompositionAlgorithm(grids, scheme)
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

        if(grids.empty())
            return new Grid();

        fillOffsets();
        fillPositions();
        fillComposedWireData();

        optimize();

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
    if(stopped) return;

    sendLog(tr("Stretching the elements to the max size."));

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
    if(stopped) return;

    sendLog(tr("Getting initial positions."));

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
    gridWidth = floor(sqrt(grids.size()));
    gridWidth *= maxSize.width();
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

void PermutationComposition::optimize()
{
    sendLog(tr("Optimization."));

    for(int i=0; i<grids.size(); i++)
    {
        if(stopped) return;

        sendLog(tr("Optimizing position of grid %1 of %2.")
                .arg(QString::number(i+1), QString::number(grids.size())));

        int positionIndex = getPositionIndexByGridIndex(i);

        QList<qint64> fitnessValues;

        for(int j=0; j<positions.size(); j++)
            fitnessValues.append(getFitnessValueWithPermutation(j, positionIndex));

        if(stopped) return;

        int optimalPositionIndex = findOptimalPositionIndex(fitnessValues);

        if(fitnessValues[optimalPositionIndex] == fitnessValues[positionIndex])
            continue;

        std::swap(positions[positionIndex].gridIndex, positions[optimalPositionIndex].gridIndex);
    }
}

int PermutationComposition::getPositionIndexByGridIndex(int gridIndex)
{
    for(int i=0; i<positions.size(); i++)
    {
        if(positions[i].gridIndex == gridIndex)
            return i;
    }

    throw IllegalArgumentException(tr("Cannot find the position of grid with index %1.")
                                   .arg(QString::number(gridIndex)));
}

qint64 PermutationComposition::getFitnessValueWithPermutation(int firstIndex, int secondIndex)
{
    if(stopped) return 0;

    if(firstIndex == secondIndex)
        return getFitnessValue(positions);

    QList<GridPosition> newPositions = positions;
    std::swap(newPositions[firstIndex].gridIndex, newPositions[secondIndex].gridIndex);

    return getFitnessValue(newPositions);
}

qint64 PermutationComposition::getFitnessValue(QList<GridPosition>& positions)
{
    qint64 res = 0;

    for(GridPartWireData& data: composedInternalWireData)
    {
        Wire wire = SchemeUtils::findWireByIndex(scheme, data.index);

        QPoint srcCoord = getActualCoord(data.srcGridIndex, data.srcCoord, positions);
        QPoint destCoord = getActualCoord(data.destGridIndex, data.destCoord, positions);

        int val = WireUtils::getDistance(srcCoord, destCoord, WirePosition::Internal, 0, 0);

        if(wire.getType() == WireType::Inner)
            val *= innerWireFitnessCoefficient;

        res += val;
    }

    return res;
}

int PermutationComposition::findOptimalPositionIndex(QList<qint64>& fitnessValues)
{
    int optimalPositionIndex = 0;
    int minFitnessValue = fitnessValues[0];

    for(int j=0; j<fitnessValues.size(); j++)
    {
        if(fitnessValues[j] < minFitnessValue)
        {
            minFitnessValue = fitnessValues[j];
            optimalPositionIndex = j;
        }
    }

    return optimalPositionIndex;
}

Grid* PermutationComposition::buildResult()
{
    if(stopped) return nullptr;

    sendLog(tr("Building the result grid."));

    Grid* grid = createEmptyTotalGrid();
    fillTotalGrid(grid);

    fillWiresData(grid);
    fillRoutedWires(grid);

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

void PermutationComposition::fillTotalGrid(Grid* result)
{
    for(GridPosition& position: positions)
    {
        QPoint coord = position.coord;
        Grid* sourceGrid = grids[position.gridIndex];

        QPoint offset = offsets[position.gridIndex];

        for(int i=0; i<sourceGrid->getHeight(); i++)
        {
            for(int j=0; j<sourceGrid->getWidth(); j++)
            {
                result->getCells()[coord.y() + offset.y() + i][coord.x() + offset.x() + j] =
                        sourceGrid->getCells()[i][j];
            }
        }
    }

}

void PermutationComposition::fillWiresData(Grid* result)
{
    for(GridPartWireData& data: composedInternalWireData)
    {        
        result->getWiresData().append(WireData(data.index, getActualCoord(data.srcGridIndex, data.srcCoord),
                                               getActualCoord(data.destGridIndex, data.destCoord), WirePosition::Internal));
    }

    for(ExternalWireData& data: composedExternalWireData)
    {
        result->getWiresData().append(WireData(data.index, getActualCoord(data.gridIndex, data.srcCoord), QPoint(0, 0), WirePosition::External));
    }

    for(int i=0; i<grids.size(); i++)
    {
        for(WireData& data: grids[i]->getWiresData())
        {
            if(data.getWirePosition() == WirePosition::Internal)
                result->getWiresData().append(WireData(data.getIndex(), getActualCoord(i, data.getSrcCoord()),
                                                       getActualCoord(i, data.getDestCoord()), WirePosition::Internal));
        }
    }
}

QPoint PermutationComposition::getActualCoord(int gridIndex, QPoint coord)
{
    return getActualCoord(gridIndex, coord, positions);
}

QPoint PermutationComposition::getActualCoord(int gridIndex, QPoint coord, QList<GridPosition>& positions)
{
    QPoint position;
    QPoint offset;

    for(GridPosition& pos: positions)
    {
        if(pos.gridIndex == gridIndex)
        {
            position = pos.coord;
            break;
        }
    }

    offset = offsets[gridIndex];

    return position + offset + coord;
}

void PermutationComposition::fillRoutedWires(Grid* result)
{
    for(Grid* g: grids)
    {
        result->getRoutedWires().append(g->getRoutedWires());
    }
}
