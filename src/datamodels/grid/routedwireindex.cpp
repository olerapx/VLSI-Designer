#include "routedwireindex.h"

RoutedWireIndex::RoutedWireIndex(qint64 index)
{
    setValue(index);
}

void RoutedWireIndex::setValue(qint64 index)
{
    if (index < 0)
        throw IllegalArgumentException(QObject::tr("Routed wire index cannot be negative, passed: %1.").arg(QString::number(index)));

    this->value = index;
}
