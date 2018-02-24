#include "internodeconnectionsnumbercomparator.h"

InterNodeConnectionsNumberComparator::InterNodeConnectionsNumberComparator(QList<SchemeVertex*> &list) :
    list(list)
{
    for(SchemeVertex* v: list)
    {
        if (v == nullptr)
            continue;

        for(std::pair<SchemeVertex*, WireType>& connectedElement: v->getConnectedElements())
        {
            if(connectedElement.second == WireType::InterNode)
                interConnectedElements.append(connectedElement.first);
        }
    }
}

bool InterNodeConnectionsNumberComparator::operator ()(SchemeVertex* v1, SchemeVertex* v2) const
{
    int firstConnectionsNumber = 0;
    int secondConnectionsNumber = 0;

    for(std::pair<SchemeVertex*, WireType>& connectedElement: v1->getConnectedElements())
    {
        if(!list.contains(connectedElement.first))
            firstConnectionsNumber ++;
    }

    for(std::pair<SchemeVertex*, WireType>& connectedElement: v2->getConnectedElements())
    {
        if(!list.contains(connectedElement.first))
            secondConnectionsNumber ++;
    }

    for(SchemeVertex* v: interConnectedElements)
    {
        if(v != v1)
        {
            firstConnectionsNumber ++;
        }
        if (v != v2)
        {
            secondConnectionsNumber ++;
        }
    }

    return (firstConnectionsNumber < secondConnectionsNumber);
}
