#include "routedwireindex.h"

RoutedWireIndex::RoutedWireIndex(qint64 index)
{
    setValue(index);
}

void RoutedWireIndex::setValue(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException("Routed wire index cannot be negative");

    this->value = index;
}
