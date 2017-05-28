#ifndef ROUTEDWIREINDEX_H
#define ROUTEDWIREINDEX_H

#include <QtGlobal>
#include <QObject>

#include "exception/illegalargumentexception.h"

/**
 * @brief The RoutedWireIndex class
 * Represents a routed wire's index contained in grid.
 */
class RoutedWireIndex
{
public:
    RoutedWireIndex(qint64 index);

    operator qint64() const { return value; }
    qint64 getValue() const { return value; }

private:
    qint64 value;

    void setValue(qint64 index);
};

#endif // ROUTEDWIREINDEX_H
