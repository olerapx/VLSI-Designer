#ifndef ELEMENTCOORDINATE_H
#define ELEMENTCOORDINATE_H

#include <QPoint>

#include "datamodels/scheme/schemeelement.h"

/**
 * @brief The ElementCoordinate class
 * Encapsulates the scheme element with its top-left coord on grid.
 */
class ElementCoordinate
{
public:
    ElementCoordinate(SchemeElement element, QPoint topLeftCoord);

    void setElement(SchemeElement element);
    void setTopLeftCoord(QPoint topLeftCoord);

    SchemeElement getElement() { return element; }
    QPoint getTopLeftCoord() { return topLeftCoord; }

private:
    SchemeElement element;
    QPoint topLeftCoord;
};

#endif // ELEMENTCOORDINATE_H
