#include "rowpermutationplacement.h"

RowPermutationPlacement::RowPermutationPlacement(PlacementResult* previous):
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

        permutateRows();
        for(int i=0; i<elementCoordinates.size(); i++)
            permutateRow(i);

        PlacementResult* res = buildResult();
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

QList<WireCoordinate> RowPermutationPlacement::fillWireCoordinates(QList<QList<ElementCoordinate> > &elementCoordinates)
{
    QList<WireCoordinate> res;

    for(Wire& w: previous->getRelatedWires())
    {
        res.append(WireCoordinate(w, elementCoordinates, previous->getLibraries()));
    }

    return res;
}

void RowPermutationPlacement::permutateRows()
{

}

void RowPermutationPlacement::permutateRow(int rowIndex)
{
    positions.clear();
    for(int i=0; i<elementCoordinates[rowIndex].size(); i++)
        positions.insert(i, i);

    for(int i=0; i<positions.size(); i++)
        findOptimalElementPosition(rowIndex, i);
}

void RowPermutationPlacement::findOptimalElementPosition(int rowIndex, int elementIndex)
{
    int elementPosition = positions[elementIndex];
    QList<qint64> fitnessDiffs;

    for(int i=0; i<positions.size(); i++)
    {
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

    ElementCoordinate& firstCoord = row[firstElementPosition];
    ElementCoordinate& secondCoord = row[secondElementPosition];

    int shift = getShift(firstCoord, secondCoord);

    QPoint secondElementCoord = secondCoord.getTopLeftCoord();
    secondCoord.setTopLeftCoord(firstCoord.getTopLeftCoord());

    firstCoord.setTopLeftCoord(QPoint(secondElementCoord.x() + shift, secondElementCoord.y()));

    for(int i=firstElementPosition + 1; i<secondElementPosition; i++)
    {
        QPoint coord = row[i].getTopLeftCoord();
        row[i].setTopLeftCoord(QPoint(coord.x() + shift, coord.y()));
    }

    row.swap(firstElementPosition, secondElementPosition);
}

int RowPermutationPlacement::getShift(ElementCoordinate first, ElementCoordinate second)
{
    LibraryElement firstLibElement = LibraryUtils::getCorrespondingElement(first.getElement(), previous->getLibraries());
    LibraryElement secondLibElement = LibraryUtils::getCorrespondingElement(second.getElement(), previous->getLibraries());

    int shift = secondLibElement.getWidth() - firstLibElement.getWidth();

    if(first.getTopLeftCoord().x() > second.getTopLeftCoord().y())
        shift = -shift;

    return shift;
}

void RowPermutationPlacement::swapElementsOnGrid(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition)
{
    QList<ElementCoordinate>& row = elementCoordinates[rowIndex];

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
        GridUtils::insertEmptyArea(previous->getGrid(), firstCoord.getTopLeftCoord(), shift, height);
        GridUtils::removeArea(previous->getGrid(), secondCoord.getTopLeftCoord(), shift, height);
    }
    else if(shift < 0)
    {
        GridUtils::insertEmptyArea(previous->getGrid(), secondCoord.getTopLeftCoord(), -shift, height);
        GridUtils::removeArea(previous->getGrid(), firstCoord.getTopLeftCoord(), -shift, height);
    }

    GridUtils::paste(previous->getGrid(), secondElementCells, firstCoord.getTopLeftCoord());
    GridUtils::paste(previous->getGrid(), firstElementCells, QPoint(secondCoord.getTopLeftCoord().x() + shift, secondCoord.getTopLeftCoord().y()));
}

int RowPermutationPlacement::getRowHeight(QList<ElementCoordinate> &row)
{
    int height = 0;
    for(ElementCoordinate& c : row)
    {
        int h = LibraryUtils::getCorrespondingElement(c.getElement(), previous->getLibraries()).getHeight();
        if(h > height)
            height = h;
    }

    return height;
}

qint64 RowPermutationPlacement::getFitnessValue(QList<WireCoordinate> &wireCoordinates)
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
