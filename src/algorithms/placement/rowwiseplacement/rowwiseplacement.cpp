#include "rowwiseplacement.h"

RowWisePlacement::RowWisePlacement(Scheme *scheme, QList<Library *> libraries, double expandCoefficient):
    PrimaryPlacementAlgorithm(scheme, libraries, expandCoefficient),
    comparator(libraries)
{
    grid = nullptr;
}

PlacementResult* RowWisePlacement::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;       

        clear();

        fillRelatedWires();
        packElements();
        expandElements();

        PlacementResult* res = new PlacementResult(grid, elementTopLeftCoords, relatedWires);
        stopped = true;
        actuallyStopped = true;

        return res;
    }
    catch(Exception e)
    {
        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void RowWisePlacement::clear()
{
    if(grid != nullptr)
    {
        delete grid;
        grid = nullptr;
    }

    elementTopLeftCoords.clear();
    packingWidth = 0;

    relatedWires.clear();

    currentPoint = QPoint(0, 0);
    firstHeightInRow = 0;

    comparator = ElementHeightComparator(libraries);
}

void RowWisePlacement::fillRelatedWires()
{
    for(Wire& w: scheme->getWires())
    {
        qint64 srcIndex = w.getSrcIndex();
        qint64 destIndex = w.getDestIndex();

        for(SchemeElement& el: scheme->getElements())
        {
            if(el.getIndex() == srcIndex || el.getIndex() == destIndex)
            {
                relatedWires.append(w);
                break;
            }
        }
    }
}

void RowWisePlacement::packElements()
{
    packingWidth = getPackingWidth();

    QList<SchemeElement> sortedElements = scheme->getElements();
    std::sort(sortedElements.begin(), sortedElements.end(), comparator);

    for(SchemeElement& el: sortedElements)
        packElement(el);
}

int RowWisePlacement::getPackingWidth()
{
    int totalSquare;

    for(SchemeElement& el: scheme->getElements())
    {
        LibraryElement libraryElement = LibraryUtils::getCorrespondingElement(el, libraries);
        totalSquare += (libraryElement.getHeight() * libraryElement.getWidth());
    }

    int width = ceil(sqrt(totalSquare));
    if(width * (width + 1) >= totalSquare)
        return width;

    return width + 1;
}

void RowWisePlacement::packElement(SchemeElement el)
{
    LibraryElement libraryElement = LibraryUtils::getCorrespondingElement(el, libraries);

    if(elementTopLeftCoords.empty())
    {
        elementTopLeftCoords.append(currentPoint);
        currentPoint.setX(libraryElement.getWidth());

        firstHeightInRow = libraryElement.getHeight();

        return;
    }

    int newX = currentPoint.x() + libraryElement.getWidth();
    if(newX <= packingWidth)
    {
       elementTopLeftCoords.append(currentPoint);
       currentPoint.setX(newX);
       return;
    }

    elementTopLeftCoords.append(QPoint(0, currentPoint.y() + firstHeightInRow));

    currentPoint.setX(libraryElement.getWidth());
    currentPoint.setY(currentPoint.y() + firstHeightInRow);

    firstHeightInRow = libraryElement.getHeight();
}

void RowWisePlacement::expandElements()
{
    int newWidth = width * expandCoefficient;

    grid = new Grid();
}
