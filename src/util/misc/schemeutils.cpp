#include "schemeutils.h"

Wire& SchemeUtils::findWireByIndex(Scheme* s, qint64 index)
{
    for(Wire& w: s->getWires())
    {
        if(w.getIndex() == index)
            return w;
    }

    throw Exception(QObject::tr("Cannot find the wire with index %1.").arg(QString::number(index)));
}
