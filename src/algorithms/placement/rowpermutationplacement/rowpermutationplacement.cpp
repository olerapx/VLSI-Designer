#include "rowpermutationplacement.h"

RowPermutationPlacement::RowPermutationPlacement(PlacementResult* previous) :
    SecondaryPlacementAlgorithm(previous)
{
    clear();
}

PlacementResult* RowPermutationPlacement::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        elementCoordinates = previous->getRowWiseCoordinates();
        wireCoordinates = fillWireCoordinates(elementCoordinates);

        sendLog(tr("Pairwise rows permutation."));
        permutateRows();

        for(int i=0; i<elementCoordinates.size(); i++)
        {
            sendLog(tr("Pairwise elements permutation in row %1 of %2.").arg(QString::number(i+1), QString::number(elementCoordinates.size())));
            permutateRow(i);
        }

        PlacementResult* res = buildResult();
        fillWiresData(res, wireCoordinates);
        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

        stopped = true;
        actuallyStopped = true;

        return res;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void RowPermutationPlacement::clear()
{
    elementCoordinates.clear();
    wireCoordinates.clear();

    positions.clear();
}

qint64 RowPermutationPlacement::getFitnessValue()
{
    QList<QList<ElementCoordinate>> elementCoordinates = previous->getRowWiseCoordinates();
    QList<WireCoordinate> wireCoordinates = fillWireCoordinates(elementCoordinates);

    return getFitnessValue(wireCoordinates);
}

void RowPermutationPlacement::permutateRows()
{
    if(stopped) return;

    positions.clear();
    for(int i=0; i<elementCoordinates.size(); i++)
        positions.insert(i, i);

    for(int i=0; i<positions.size(); i++)
    {
        if(stopped) return;

        sendLog(tr("Finding the optimal position of row %1 of %2.").arg(QString::number(i+1), QString::number(positions.size())));

        findOptimalRowPosition(i);
    }
}

void RowPermutationPlacement::findOptimalRowPosition(int rowIndex)
{
    int rowPosition = positions[rowIndex];

    QList<qint64> fitnessDiffs;

    for(int i=0; i<positions.size(); i++)
    {
        if(stopped) return;

        if(i == rowIndex)
        {
            fitnessDiffs.append(0);
            continue;
        }

        int secondRowPosition = positions[i];

        fitnessDiffs.append(findFitnessDiffOnRowsSwapping(rowPosition, secondRowPosition));
    }

    int maxDiffIndex = 0;
    qint64 maxDiff = 0;

    for(int i=0; i<fitnessDiffs.size(); i++)
    {
        if(fitnessDiffs[i] > maxDiff)
        {
            maxDiff = fitnessDiffs[i];
            maxDiffIndex = i;
        }
    }

    if(maxDiff == 0)
        return;

    if(stopped) return;

    sendLog(tr("Performing permutation..."));

    swapRowsCoordinates(elementCoordinates, rowPosition, positions[maxDiffIndex]);

    positions[rowIndex] = positions[maxDiffIndex];
    positions[maxDiffIndex] = rowPosition;
}

qint64 RowPermutationPlacement::findFitnessDiffOnRowsSwapping(int firstRowPosition, int secondRowPosition)
{
    QList<QList<ElementCoordinate>> newElementCoordinates = elementCoordinates;

    swapRowsCoordinates(newElementCoordinates, firstRowPosition, secondRowPosition);
    QList<WireCoordinate> newWireCoordinates = fillWireCoordinates(newElementCoordinates);

    qint64 fitnessValue = getFitnessValue(wireCoordinates);
    qint64 newFitnessValue = getFitnessValue(newWireCoordinates);

    return (fitnessValue - newFitnessValue);
}

void RowPermutationPlacement::swapRowsCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates, int firstRowPosition, int secondRowPosition)
{
    if(firstRowPosition > secondRowPosition)
        std::swap(firstRowPosition, secondRowPosition);

    QList<ElementCoordinate>& firstRow = elementCoordinates[firstRowPosition];
    QList<ElementCoordinate>& secondRow = elementCoordinates[secondRowPosition];

    int shift = getShift(firstRow, secondRow);

    QPoint secondRowCoord = secondRow[0].getTopLeftCoord();

    for(ElementCoordinate& c: secondRow)
    {
        QPoint coord = c.getTopLeftCoord();
        c.setTopLeftCoord(QPoint(coord.x(), firstRow[0].getTopLeftCoord().y()));
    }

    for(ElementCoordinate& c: firstRow)
    {
        QPoint coord = c.getTopLeftCoord();
        c.setTopLeftCoord(QPoint(coord.x(), secondRowCoord.y() + shift));
    }

    for(int i=firstRowPosition + 1; i<secondRowPosition; i++)
    {
        for(ElementCoordinate& c: elementCoordinates[i])
        {
            QPoint coord = c.getTopLeftCoord();
            c.setTopLeftCoord(coord + QPoint(0, shift));
        }
    }

    elementCoordinates.swap(firstRowPosition, secondRowPosition);
}

int RowPermutationPlacement::getShift(QList<ElementCoordinate>& first, QList<ElementCoordinate>& second)
{
    LibraryElement firstLibElement = LibraryUtils::getCorrespondingElement(first[0].getElement(), previous->getLibraries());
    LibraryElement secondLibElement = LibraryUtils::getCorrespondingElement(second[0].getElement(), previous->getLibraries());

    int shift = secondLibElement.getHeight() - firstLibElement.getHeight();

    return shift;
}

void RowPermutationPlacement::permutateRow(int rowIndex)
{
    if(stopped) return;

    positions.clear();
    for(int i=0; i<elementCoordinates[rowIndex].size(); i++)
        positions.insert(i, i);

    for(int i=0; i<positions.size(); i++)
    {
        if(stopped) return;

        sendLog(tr("Finding the optimal position of element %1 of %2.").arg(QString::number(i+1), QString::number(positions.size())));

        findOptimalElementPosition(rowIndex, i);
    }
}

void RowPermutationPlacement::findOptimalElementPosition(int rowIndex, int elementIndex)
{
    int elementPosition = positions[elementIndex];
    QList<qint64> fitnessDiffs;

    for(int i=0; i<positions.size(); i++)
    {
        if(stopped) return;

        if(i == elementIndex)
        {
            fitnessDiffs.append(0);
            continue;
        }

        int secondElementPosition = positions[i];
        fitnessDiffs.append(findFitnessDiffOnElementsSwapping(rowIndex, elementPosition, secondElementPosition));
    }

    int maxDiffIndex = 0;
    qint64 maxDiff = 0;

    for(int i=0; i<fitnessDiffs.size(); i++)
    {
        if(fitnessDiffs[i] > maxDiff)
        {
            maxDiff = fitnessDiffs[i];
            maxDiffIndex = i;
        }
    }

    if(maxDiff == 0)
        return;

    if(stopped) return;

    sendLog(tr("Performing permutation..."));

    swapElementsCoordinates(elementCoordinates, rowIndex, elementPosition, positions[maxDiffIndex]);

    positions[elementIndex] = positions[maxDiffIndex];
    positions[maxDiffIndex] = elementPosition;
}

qint64 RowPermutationPlacement::findFitnessDiffOnElementsSwapping(int rowIndex, int firstElementPosition, int secondElementPosition)
{
    QList<QList<ElementCoordinate>> newElementCoordinates = elementCoordinates;

    swapElementsCoordinates(newElementCoordinates, rowIndex, firstElementPosition, secondElementPosition);
    QList<WireCoordinate> newWireCoordinates = fillWireCoordinates(newElementCoordinates);

    qint64 fitnessValue = getFitnessValue(wireCoordinates);
    qint64 newFitnessValue = getFitnessValue(newWireCoordinates);

    return (fitnessValue - newFitnessValue);
}

void RowPermutationPlacement::swapElementsCoordinates(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition)
{
    QList<ElementCoordinate>& row = elementCoordinates[rowIndex];

    if(firstElementPosition > secondElementPosition)
        std::swap(firstElementPosition, secondElementPosition);

    ElementCoordinate& firstCoord = row[firstElementPosition];
    ElementCoordinate& secondCoord = row[secondElementPosition];

    int shift = getShift(firstCoord, secondCoord);

    QPoint secondElementCoord = secondCoord.getTopLeftCoord();
    secondCoord.setTopLeftCoord(firstCoord.getTopLeftCoord());

    firstCoord.setTopLeftCoord(secondElementCoord + QPoint(shift, 0));

    for(int i=firstElementPosition + 1; i<secondElementPosition; i++)
    {
        QPoint coord = row[i].getTopLeftCoord();
        row[i].setTopLeftCoord(coord + QPoint(shift, 0));
    }

    row.swap(firstElementPosition, secondElementPosition);
}

int RowPermutationPlacement::getShift(ElementCoordinate first, ElementCoordinate second)
{
    LibraryElement firstLibElement = LibraryUtils::getCorrespondingElement(first.getElement(), previous->getLibraries());
    LibraryElement secondLibElement = LibraryUtils::getCorrespondingElement(second.getElement(), previous->getLibraries());

    int shift = secondLibElement.getWidth() - firstLibElement.getWidth();

    return shift;
}

qint64 RowPermutationPlacement::getFitnessValue(QList<WireCoordinate>& wireCoordinates)
{
    qint64 res = 0;
    for(WireCoordinate& w: wireCoordinates)
        res += w.getFitnessValue();

    return res;
}

PlacementResult* RowPermutationPlacement::buildResult()
{
    rebuildGrid();
    QList<ElementCoordinate> elementCoords = concatElementCoordinates();
    return new PlacementResult(previous->getGrid(), elementCoords, previous->getRelatedWires(), previous->getLibraries());
}

void RowPermutationPlacement::rebuildGrid()
{
    if(stopped) return;

    sendLog(tr("Rebuilding the grid."));

    for(QList<Cell>& row: previous->getGrid()->getCells())
    {
        for(Cell& cell: row)
        {
            cell.setType(CellType::Empty);
        }
    }

    for(QList<ElementCoordinate>& list: elementCoordinates)
    {
        for(ElementCoordinate& coord: list)
        {
            GridUtils::drawElement(previous->getGrid(), coord.getTopLeftCoord(), coord.getElement(), previous->getLibraries());
        }
    }
}

QList<ElementCoordinate> RowPermutationPlacement::concatElementCoordinates()
{
    QList<ElementCoordinate> res;

    for(QList<ElementCoordinate>& list: elementCoordinates)
        res.append(list);

    return res;
}
