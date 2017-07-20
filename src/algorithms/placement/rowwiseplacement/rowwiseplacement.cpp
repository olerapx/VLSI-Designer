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
    catch(...)
    {
        clear();

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
    totalRowWidths.clear();

    comparator = ElementHeightComparator(libraries);
}

void RowWisePlacement::packElements()
{
    if(stopped) return;
    sendLog(tr("Packing the elements."));

    packingWidth = getPackingWidth();

    sortedElements = scheme->getElements();
    std::sort(sortedElements.begin(), sortedElements.end(), comparator);

    int size = sortedElements.size();
    int i = 1;

    for(SchemeElement& el: sortedElements)
    {
        if(stopped) return;
        sendLog(tr("Packing element %1 of %2.").arg(QString::number(i++), QString::number(size)));

        packElement(el);
    }
}

int RowWisePlacement::getPackingWidth()
{
    int totalSquare = 0;

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
        totalRowWidths.append(libraryElement.getWidth());

        currentPoint.setX(libraryElement.getWidth());

        firstHeightInRow = libraryElement.getHeight();

        return;
    }

    int newX = currentPoint.x() + libraryElement.getWidth();
    if(newX <= packingWidth)
    {
       elementTopLeftCoords.last().append(currentPoint);
       totalRowWidths.last() = totalRowWidths.last() + libraryElement.getWidth();
       currentPoint.setX(newX);
       return;
    }

    elementTopLeftCoords.append(QList<QPoint>());
    elementTopLeftCoords.last().append(QPoint(0, currentPoint.y() + firstHeightInRow));

    totalRowsHeight += libraryElement.getHeight();
    totalRowWidths.append(libraryElement.getWidth());

    currentPoint.setX(libraryElement.getWidth());
    currentPoint.setY(currentPoint.y() + firstHeightInRow);

    firstHeightInRow = libraryElement.getHeight();
}

void RowWisePlacement::expandElements()
{
    if(stopped) return;
    sendLog(tr("Expanding the elements."));

    expandRows();

    int size = elementTopLeftCoords.size();

    for(int i=0; i<size; i++)
    {
        if(stopped) return;
        sendLog(tr("Expanding row %1 of %2.").arg(QString::number(i+1), QString::number(size)));
        expandRow(elementTopLeftCoords[i], totalRowWidths[i]);
    }
}

void RowWisePlacement::expandRows()
{
    if(stopped) return;
    sendLog(tr("Expanding rows."));

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

    int quotient = diff / count;
    int reminder = diff % count;

    QVector<int> intervals(count + 1);

    for(int i=0; i<count; i++)
        intervals[i] = quotient;

    for(int i=0; i<reminder; i++)
        intervals[i] ++;

    intervals[count] = intervals[0] / 2;
    intervals[0] = intervals[0] / 2 + intervals[0] % 2;

    return intervals;
}

void RowWisePlacement::expandRow(QList<QPoint> &list, int width)
{
    int count = list.size();
    QVector<int> intervals = getIntervals(width, count);

    int currentShift = 0;

    for(int i=0; i<count; i++)
    {
        currentShift += intervals[i];
        list[i].setX(list[i].x() + currentShift);
    }
}

PlacementResult* RowWisePlacement::buildResult()
{
    if(stopped) return nullptr;
    sendLog(tr("Building the result."));

    QList<QPoint> topLeftCoords = concatTopLeftCoords();
    buildGrid(topLeftCoords);
    fillRelatedWires();

    return new PlacementResult(grid, elementTopLeftCoords, relatedWires);
}

QList<QPoint> RowWisePlacement::concatTopLeftCoords()
{
    QList<QPoint> res;

    for(QList<QPoint>& list: elementTopLeftCoords)
        res += list;

    return res;
}

void RowWisePlacement::buildGrid(QList<QPoint>& topLeftCoords)
{
    if(stopped) return;
    sendLog(tr("Building the result grid."));

    grid = new Grid();

    int currentIndex = 0;

    int totalWidth = round(packingWidth * expandCoefficient);
    int totalHeight = round(totalRowsHeight * expandCoefficient);

    for(int i=0; i< totalHeight; i++)
    {
        if(stopped) return;

        QList<Cell> list;

        for(int j=0; j<totalWidth; j++)
            list.append(Cell(CellType::Empty));

        grid->getCells().append(list);
    }

    for(QPoint point: topLeftCoords)
    {
        if(stopped) return;

        drawElement(point, sortedElements[currentIndex]);
        currentIndex ++;
    }
}

void RowWisePlacement::drawElement(QPoint point, SchemeElement element)
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

void RowWisePlacement::fillRelatedWires()
{
    if(stopped) return;
    sendLog(tr("Distinguishing the scheme-related wires."));

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

