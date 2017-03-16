#ifndef ROUTEDWIREINDEX_H
#define ROUTEDWIREINDEX_H

#include <QtGlobal>

#include "exception/illegalargumentexception.h"

/**
 * @brief The RoutedWireIndex class
 * Represents a routed wire's index contained in grid.
 */
class RoutedWireIndex
{
private:
    qint64 value;

    void setValue (qint64 index);
public:
    RoutedWireIndex(qint64 index);

    operator qint64() const {return this->value;}
    qint64 getValue() const {return this->value;}
};

#endif // ROUTEDWIREINDEX_H
