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
    QList<QList<ElementCoordinate>> elementCoords = previous->getRowWiseCoordinates();
    QList<WireCoordinate> coordinates = fillWireCoordinates(elementCoords);

    return getFitnessValue(coordinates);
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

    swapRowsOnGrid(elementCoordinates, rowPosition, positions[maxDiffIndex]);
    swapRowsCoordinates(elementCoordinates, rowPosition, positions[maxDiffIndex]);

    positions[rowIndex] = positions[maxDiffIndex];
    positions[maxDiffIndex] = rowPosition;
}

qint64 RowPermutationPlacement::findFitnessDiffOnRowsSwapping(int firstRowPosition, int secondRowPosition)
{
    qint64 fitnessValue = getFitnessValue(wireCoordinates);

    QList<QList<ElementCoordinate>> newElementCoordinates = elementCoordinates;
    swapRowsCoordinates(newElementCoordinates, firstRowPosition, secondRowPosition);

    QList<WireCoordinate> newWireCoordinates = fillWireCoordinates(newElementCoordinates);

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
            c.setTopLeftCoord(QPoint(coord.x(), coord.y() + shift));
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

void RowPermutationPlacement::swapRowsOnGrid(QList<QList<ElementCoordinate>>& elementCoordinates, int firstRowPosition, int secondRowPosition)
{
    if(firstRowPosition > secondRowPosition)
        std::swap(firstRowPosition, secondRowPosition);

    QList<ElementCoordinate>& firstRow = elementCoordinates[firstRowPosition];
    QList<ElementCoordinate>& secondRow = elementCoordinates[secondRowPosition];

    QPoint firstCoord = firstRow[0].getTopLeftCoord();
    QPoint secondCoord = secondRow[0].getTopLeftCoord();

    int shift = getShift(firstRow, secondRow);
    int width = previous->getGrid()->getCells()[0].size();

    int firstHeight = LibraryUtils::getCorrespondingElement(firstRow[0].getElement(), previous->getLibraries()).getHeight();
    int secondHeight = LibraryUtils::getCorrespondingElement(secondRow[0].getElement(), previous->getLibraries()).getHeight();

    QList<QList<Cell>> firstRowCells = GridUtils::cut(previous->getGrid(), QPoint(0, firstCoord.y()), width, firstHeight);
    QList<QList<Cell>> secondRowCells = GridUtils::cut(previous->getGrid(), QPoint(0, secondCoord.y()), width, secondHeight);

    if(shift > 0)
    {
        GridUtils::insertRows(previous->getGrid(), firstCoord.y(), shift);
        GridUtils::removeRows(previous->getGrid(), secondCoord.y(), shift);
    }
    else if(shift < 0)
    {
        GridUtils::insertRows(previous->getGrid(), secondCoord.y(), -shift);
        GridUtils::removeRows(previous->getGrid(), firstCoord.y(), -shift);
    }

    GridUtils::paste(previous->getGrid(), secondRowCells, QPoint(0, firstCoord.y()));
    GridUtils::paste(previous->getGrid(), firstRowCells, QPoint(0, secondCoord.y() + shift));
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

    swapElementsOnGrid(elementCoordinates, rowIndex, elementPosition, positions[maxDiffIndex]);
    swapElementsCoordinates(elementCoordinates, rowIndex, elementPosition, positions[maxDiffIndex]);

    positions[elementIndex] = positions[maxDiffIndex];
    positions[maxDiffIndex] = elementPosition;
}

qint64 RowPermutationPlacement::findFitnessDiffOnElementsSwapping(int rowIndex, int firstElementPosition, int secondElementPosition)
{
    qint64 fitnessValue = getFitnessValue(wireCoordinates);

    QList<QList<ElementCoordinate>> newElementCoordinates = elementCoordinates;
    swapElementsCoordinates(newElementCoordinates, rowIndex, firstElementPosition, secondElementPosition);

    QList<WireCoordinate> newWireCoordinates = fillWireCoordinates(newElementCoordinates);

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

void RowPermutationPlacement::swapElementsOnGrid(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition)
{
    QList<ElementCoordinate>& row = elementCoordinates[rowIndex];

    if(firstElementPosition > secondElementPosition)
        std::swap(firstElementPosition, secondElementPosition);

    ElementCoordinate& firstCoord = row[firstElementPosition];
    ElementCoordinate& secondCoord = row[secondElementPosition];

    int shift = getShift(firstCoord, secondCoord);

    int height = getRowHeight(row);

    LibraryElement firstLibElement = LibraryUtils::getCorrespondingElement(firstCoord.getElement(), previous->getLibraries());
    LibraryElement secondLibElement = LibraryUtils::getCorrespondingElement(secondCoord.getElement(), previous->getLibraries());

    QList<QList<Cell>> firstElementCells = GridUtils::cut(previous->getGrid(), firstCoord.getTopLeftCoord(), firstLibElement.getWidth(), height);
    QList<QList<Cell>> secondElementCells = GridUtils::cut(previous->getGrid(), secondCoord.getTopLeftCoord(), secondLibElement.getWidth(), height);

    if(shift > 0)
    {
        GridUtils::insertEmptyColumns(previous->getGrid(), firstCoord.getTopLeftCoord(), shift, height);
        GridUtils::removeColumns(previous->getGrid(), secondCoord.getTopLeftCoord(), shift, height);
    }
    else if(shift < 0)
    {
        GridUtils::insertEmptyColumns(previous->getGrid(), secondCoord.getTopLeftCoord(), -shift, height);
        GridUtils::removeColumns(previous->getGrid(), firstCoord.getTopLeftCoord(), -shift, height);
    }

    GridUtils::paste(previous->getGrid(), secondElementCells, firstCoord.getTopLeftCoord());
    GridUtils::paste(previous->getGrid(), firstElementCells, (secondCoord.getTopLeftCoord() + QPoint(shift, 0)));
}

int RowPermutationPlacement::getRowHeight(QList<ElementCoordinate>& row)
{
    int height = 0;
    for(ElementCoordinate& c: row)
    {
        int h = LibraryUtils::getCorrespondingElement(c.getElement(), previous->getLibraries()).getHeight();
        if(h > height)
            height = h;
    }

    return height;
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
    QList<ElementCoordinate> elementCoords = concatCoordinates();
    return new PlacementResult(previous->getGrid(), elementCoords, previous->getRelatedWires(), previous->getLibraries());
}

QList<ElementCoordinate> RowPermutationPlacement::concatCoordinates()
{
    QList<ElementCoordinate> res;

    for(QList<ElementCoordinate>& list: elementCoordinates)
        res.append(list);

    return res;
}
