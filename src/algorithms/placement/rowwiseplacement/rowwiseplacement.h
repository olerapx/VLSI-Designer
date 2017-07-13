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
    QList<QPoint> elementTopLeftCoords;
    QList<Wire> relatedWires;

    int packingWidth;

    QPoint currentPoint;
    int firstHeightInRow;

    //ADD rows and cols data: count, total size
    // or qlist qlist qpoint elementTopLeftCoords
    ElementHeightComparator comparator;

    void clear();
    void fillRelatedWires();

    void packElements();
    int getPackingWidth();
    void packElement(SchemeElement el);

    void expandElements();
};

#endif // ROWWISEPLACEMENT_H
