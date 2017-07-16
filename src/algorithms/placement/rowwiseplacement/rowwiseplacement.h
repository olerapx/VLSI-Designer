#ifndef ROWWISEPLACEMENT_H
#define ROWWISEPLACEMENT_H

#include <cmath>

#include "algorithms/placement/primaryplacementalgorithm.h"
#include "util/libraryutils.h"
#include "elementheightcomparator.h"

/**
 * @brief The RowWisePlacement class
 * Performs the row-wise placement.
 * All elements will be grouped by their height and placed in rows.
 */
class RowWisePlacement: public PrimaryPlacementAlgorithm
{
public:
    RowWisePlacement(Scheme* scheme, QList<Library*> libraries, double expandCoefficient);

    PlacementResult* execute();

private:
    Grid* grid;
    QList<QList<QPoint>> elementTopLeftCoords;
    QList<Wire> relatedWires;

    QList<SchemeElement> sortedElements;

    int packingWidth;

    QPoint currentPoint;
    int firstHeightInRow;

    int totalRowsHeight;
    QList<int> totalRowWidths;

    ElementHeightComparator comparator;

    void clear();

    void packElements();
    int getPackingWidth();
    void packElement(SchemeElement el);

    void expandElements();
    void expandRows();
    QVector<int> getIntervals(int totalSize, int count);
    void expandRow(QList<QPoint>& list, int width);

    PlacementResult* buildResult();
    QList<QPoint> concatTopLeftCoords();
    void buildGrid(QList<QPoint>& topLeftCoords);
    void drawElement(QPoint point, SchemeElement element);
    void fillRelatedWires();
};

#endif // ROWWISEPLACEMENT_H
