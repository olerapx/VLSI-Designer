#include "rowwiseplacement.h"

RowWisePlacement::RowWisePlacement(Scheme *scheme, QList<Library *> libraries, double expandCoefficient):
    PrimaryPlacementAlgorithm(scheme, libraries, expandCoefficient),
    comparator(libraries)
{
    clear();
}

PlacementResult* RowWisePlacement::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;       

        packElements();
        expandElements();

        PlacementResult* res = buildResult();
        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

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
    grid = nullptr;

    elementTopLeftCoords.clear();
    packingWidth = 0;

    relatedWires.clear();
    sortedElements.clear();

    currentPoint = QPoint(0, 0);
    firstHeightInRow = 0;

    totalRowsHeight = 0;

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

    sortedElements = scheme->getElements();
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

    int width = floor(sqrt(totalSquare));
    if(width * (width + 1) >= totalSquare)
        return width;

    return width + 1;
}

void RowWisePlacement::packElement(SchemeElement el)
{
    LibraryElement libraryElement = LibraryUtils::getCorrespondingElement(el, libraries);

    if(elementTopLeftCoords.empty())
    {
        elementTopLeftCoords.append(QList<QPoint>());
        elementTopLeftCoords.last().append(currentPoint);
        totalRowsHeight += libraryElement.getHeight();

        currentPoint.setX(libraryElement.getWidth());

        firstHeightInRow = libraryElement.getHeight();

        return;
    }

    int newX = currentPoint.x() + libraryElement.getWidth();
    if(newX <= packingWidth)
    {
       elementTopLeftCoords.last().append(currentPoint);
       currentPoint.setX(newX);
       return;
    }

    elementTopLeftCoords.append(QList<QPoint>());
    elementTopLeftCoords.last().append(QPoint(0, currentPoint.y() + firstHeightInRow));
    totalRowsHeight += libraryElement.getHeight();

    currentPoint.setX(libraryElement.getWidth());
    currentPoint.setY(currentPoint.y() + firstHeightInRow);

    firstHeightInRow = libraryElement.getHeight();
}

void RowWisePlacement::expandElements()
{
    expandRows();

    for(QList<QPoint>& list: elementTopLeftCoords)
    {
        expandRow(list);
    }
}

void RowWisePlacement::expandRows()
{
    int rowsCount = elementTopLeftCoords.size();
    QVector<int> intervals = getIntervals(totalRowsHeight, rowsCount);

    int currentShift = 0;

    for(int i=0; i<rowsCount; i++)
    {
        currentShift += intervals[i];
        for(QPoint& point: elementTopLeftCoords[i])
        {
            point.setY(point.y() + currentShift);
        }
    }
}

QVector<int> RowWisePlacement::getIntervals(int totalSize, int count)
{
    int diff = round(totalSize * expandCoefficient) - totalSize;

    double interval = (double) diff / (count);

    QVector<int> intervals(count + 1);

    intervals[0] = intervals[count] = floor(interval) / 2;
    diff -= floor(interval);
    interval = (double) diff / (count - 2);

    for(int i=1; i<count-1; i++)
    {
        int actualInterval = 0;
        if(i % 2)
            actualInterval = ceil(interval);
        else
            actualInterval = floor(interval);

        intervals[i] = actualInterval;
        diff -= actualInterval;
    }

    intervals[count - 1] = diff;

    return intervals;
}

void RowWisePlacement::expandRow(QList<QPoint> &list)
{
    int count = list.size();
    QVector<int> intervals = getIntervals(packingWidth, count);

    int currentShift = 0;

    for(int i=0; i<count; i++)
    {
        currentShift += intervals[i];
        list[i].setX(list[i].x() + currentShift);
    }
}

PlacementResult* RowWisePlacement::buildResult()
{
    QList<QPoint> topLeftCoords = buildTopLeftCoords();
    buildGrid(topLeftCoords);
    fillRelatedWires();

    return new PlacementResult(grid, topLeftCoords, relatedWires);
}

QList<QPoint> RowWisePlacement::buildTopLeftCoords()
{
    QList<QPoint> res;

    for(QList<QPoint>& list: elementTopLeftCoords)
        res += list;

    return res;
}

void RowWisePlacement::buildGrid(QList<QPoint>& topLeftCoords)
{
    grid = new Grid();

    int currentIndex = 0;

    int totalWidth = round(packingWidth * expandCoefficient);
    int totalHeight = round(totalRowsHeight * expandCoefficient);

    for(int i=0; i< totalHeight; i++)
    {
        QList<Cell> list;

        for(int j=0; j<totalWidth; j++)
            list.append(Cell(CellType::Empty));

        grid->getCells().append(list);
    }

    for(QPoint point: topLeftCoords)
    {
        drawElement(point, sortedElements[currentIndex]);
        currentIndex ++;
    }
}

void RowWisePlacement::drawElement(QPoint point, SchemeElement element)
{
    LibraryElement libraryElement = LibraryUtils::getCorrespondingElement(element, libraries);

    for(int i=0; i<libraryElement.getHeight(); i++)
    {
        for(int j=0; j<libraryElement.getWidth(); j++)
        {
            grid->getCells()[point.x() + i][point.y() + j] = Cell(CellType::Element, element.getIndex());
        }
    }

    for(Pin p: libraryElement.getPins())
    {
        grid->getCells()[point.x() + p.getX()][point.y() + p.getY()] = Cell(CellType::Pin, element.getIndex(), p.getId());
    }
}
