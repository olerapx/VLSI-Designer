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

        PlacementResult* res = nullptr;
        clear();

        elementCoordinates = previous->getRowWiseCoordinates();
        wireCoordinates = fillWireCoordinates(previous->getElementCoordinates());

        permutateRows();
        for(int i=0; i<elementCoordinates.size(); i++)
            permutateRow(i);

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

QList<WireCoordinate> RowPermutationPlacement::fillWireCoordinates(QList<ElementCoordinate>& elementCoordinates)
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
    QList<int> fitnessDiffs;

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
    int maxDiff = 0;

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

    swapElements(elementCoordinates, rowIndex, elementPosition, positions[maxDiffIndex]);

    positions[elementIndex] = positions[maxDiffIndex];
    positions[maxDiffIndex] = elementPosition;
}

int RowPermutationPlacement::findFitnessDiffOnElementsSwapping(int rowIndex, int firstElementPosition, int secondElementPosition)
{
    int fitnessValue = getFitnessValue(wireCoordinates);

    QList<QList<ElementCoordinate>> newRowWiseCoordinates = elementCoordinates;
    swapElements(newRowWiseCoordinates, rowIndex, firstElementPosition, secondElementPosition);

    QList<ElementCoordinate> newElementCoordinates;
    for(QList<ElementCoordinate>& list: newRowWiseCoordinates)
        newElementCoordinates.append(list);

    QList<WireCoordinate> newWireCoordinates = fillWireCoordinates(newElementCoordinates);

    int newFitnessValue = getFitnessValue(newWireCoordinates);

    return (fitnessValue - newFitnessValue);
}

void RowPermutationPlacement::swapElements(QList<QList<ElementCoordinate>>& elementCoordinates, int rowIndex, int firstElementPosition, int secondElementPosition)
{
    QList<ElementCoordinate>& row = elementCoordinates[rowIndex];

    ElementCoordinate& firstCoord = row[firstElementPosition];
    ElementCoordinate& secondCoord = row[secondElementPosition];

    LibraryElement firstLibElement = LibraryUtils::getCorrespondingElement(firstCoord.getElement(), previous->getLibraries());
    LibraryElement secondLibElement = LibraryUtils::getCorrespondingElement(secondCoord.getElement(), previous->getLibraries());

    int shift = secondLibElement.getWidth() - firstLibElement.getWidth();

    if(firstCoord.getTopLeftCoord().x() > secondCoord.getTopLeftCoord().y())
        shift = -shift;

    int height = getRowHeight(row);

    QList<QList<Cell>> firstElementCells = GridUtils::cut(previous->getGrid(), firstCoord.getTopLeftCoord(), firstLibElement.getWidth(), height);
    QList<QList<Cell>> secondElementCells = GridUtils::cut(previous->getGrid(), secondCoord.getTopLeftCoord(), secondLibElement.getWidth(), height);

    if(shift >= 0)
    {
        GridUtils::insertEmptyArea(previous->getGrid(), secondCoord.getTopLeftCoord(), shift, height);
        GridUtils::removeArea(previous->getGrid(), firstCoord.getTopLeftCoord(), shift, height);
    }
    else
    {
        GridUtils::insertEmptyArea(previous->getGrid(), firstCoord.getTopLeftCoord(), -shift, height);
        GridUtils::removeArea(previous->getGrid(), secondCoord.getTopLeftCoord(), -shift, height);
    }

    GridUtils::paste(previous->getGrid(), secondElementCells, firstCoord.getTopLeftCoord());
    GridUtils::paste(previous->getGrid(), firstElementCells, QPoint(firstCoord.getTopLeftCoord().x() + shift, firstCoord.getTopLeftCoord().y()));

    QPoint secondElementCoord = secondCoord.getTopLeftCoord();
    secondCoord.setTopLeftCoord(firstCoord.getTopLeftCoord());

    firstCoord.setTopLeftCoord(QPoint(secondElementCoord.x() + shift, secondElementCoord.y()));

    for(int i=firstElementPosition + 1; i<secondElementPosition; i++)
    {
        QPoint coord = row[i].getTopLeftCoord();
        row[i].setTopLeftCoord(QPoint(coord.x() + shift, coord.y()));
    }
}

int RowPermutationPlacement::getRowHeight(QList<ElementCoordinate> &row)
{
    int height = 0;
    for(ElementCoordinate&c : row)
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
