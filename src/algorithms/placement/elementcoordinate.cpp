#include "elementcoordinate.h"

ElementCoordinate::ElementCoordinate(SchemeElement element, QPoint topLeftCoord) :
    element(element)
{
    setTopLeftCoord(topLeftCoord);
}

void ElementCoordinate::setElement(SchemeElement element)
{
    this->element = element;
}

void ElementCoordinate::setTopLeftCoord(QPoint topLeftCoord)
{
    if(topLeftCoord.x() < 0 || topLeftCoord.y() < 0)
        throw IllegalArgumentException(QObject::tr("Element coordinates cannot be negative, passed: (%1; %2).")
                                       .arg(QString::number(topLeftCoord.x()), QString::number(topLeftCoord.y())));

    this->topLeftCoord = topLeftCoord;
}
