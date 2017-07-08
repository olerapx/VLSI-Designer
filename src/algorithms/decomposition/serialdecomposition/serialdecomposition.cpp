#include "serialdecomposition.h"

SerialDecomposition::SerialDecomposition(Scheme* scheme, int number):
    DecompositionAlgorithm(scheme, number)
{

}

QList<Scheme*> SerialDecomposition::execute()
{
    if(!actuallyStopped)
    {
        sendError(tr("The algorithm is already working."));
        return QList<Scheme*>();
    }

    stopped = false;
    actuallyStopped = false;

    try
    {
        clear();
        fillVertices();
        prepareSubschemes();

        for(QList<SchemeVertex*>& list: distributedVertices)
            fillSubscheme(list);

        QList<Scheme*> list = buildSubschemes();

        stopped = true;
        actuallyStopped = true;

        sendSchemes(list);
        sendFinish();

        return list;
    }
    catch(Exception e)
    {
        stopped = true;
        actuallyStopped = true;

        sendError(e.what());
        sendFinish();
        return QList<Scheme*>();
    }
}

void SerialDecomposition::clear()
{
    for(SchemeVertex* v: undistributedVertices)
        delete v;

    undistributedVertices.clear();

    for(QList<SchemeVertex*>& list: distributedVertices)
    {
        for(SchemeVertex* v: list)
            delete v;

        list.clear();
    }
    distributedVertices.clear();
}

void SerialDecomposition::fillVertices()
{
    for(SchemeElement& el: scheme->getElements())
        undistributedVertices.append(new SchemeVertex(&el));

    for(Wire& w: scheme->getWires())
    {
        SchemeVertex* srcVertex = findVertexByIndex(w.getSrcIndex());
        SchemeVertex* destVertex = findVertexByIndex(w.getDestIndex());

        srcVertex->getConnectedElements().append(std::make_pair(destVertex, w.getType()));
        destVertex->getConnectedElements().append(std::make_pair(srcVertex, w.getType()));
    }
}

SchemeVertex* SerialDecomposition::findVertexByIndex(qint64 index)
{
    for(SchemeVertex* v: undistributedVertices)
    {
        if(v->getElement()->getIndex() == index)
            return v;
    }

    throw IllegalArgumentException(tr("Cannot find scheme element with index %1.").arg(QString::number(index)));
}

void SerialDecomposition::prepareSubschemes()
{
    qint64 quotient = scheme->getElements().size() / number;
    qint64 reminder = scheme->getElements().size() % number;

    if(reminder == 0)
    {
        for(int i=0; i < number; i++)
        {
            QList<SchemeVertex*> list;
            for(int j=0; j< quotient; j++)
                list.append(nullptr);

            distributedVertices.append(list);
        }

        return;
    }

    for(int i=0; i < number-1; i++)
    {
        QList<SchemeVertex*> list;
        for(int j=0; j< quotient; j++)
            list.append(nullptr);

        distributedVertices.append(list);
    }

    QList<SchemeVertex*> list;
    for(int i=0; i< quotient + reminder; i++)
        list.append(nullptr);

    distributedVertices.append(list);
}

void SerialDecomposition::fillSubscheme(QList<SchemeVertex*>& list)
{
    placeFirstElement(list);

    for(int i=1; i<list.size(); i++)
    {
        placeNextElement(list, i);
    }
}

void SerialDecomposition::placeFirstElement(QList<SchemeVertex*> &list)
{
    std::sort(undistributedVertices.begin(), undistributedVertices.end(), SerialDecomposition::connectedElementsNumberComparator);

    list[0] = undistributedVertices[0];
    undistributedVertices.removeFirst();
    list[0]->setDistributed(true);
}

bool SerialDecomposition::connectedElementsNumberComparator(SchemeVertex* v1, SchemeVertex* v2)
{
    return (v1->getConnectedElements().size() < v2->getConnectedElements().size());
}

void SerialDecomposition::placeNextElement(QList<SchemeVertex *> &list, int index)
{
    QList<SchemeVertex*> selectableVertices = getSelectableVertices(list);
    std::sort(selectableVertices.begin(), selectableVertices.end(), OuterConnectionsNumberComparator(&list));

    SchemeVertex* vertex = selectableVertices[0];
    list[index] = vertex;
    vertex->setDistributed(true);

    int i = undistributedVertices.indexOf(vertex);
    if(i == -1)
        throw IllegalArgumentException(tr("The vertex with id %1 is already distributed or does not exist.")
                                       .arg(QString::number(vertex->getElement()->getIndex())));

    undistributedVertices.removeAt(i);
}

QList<SchemeVertex*> SerialDecomposition::getSelectableVertices(QList<SchemeVertex *> &list)
{
    QList<SchemeVertex*> selectableElements;
    for(SchemeVertex* vertex: list)
    {
        if(vertex == nullptr)
            break;

        for(std::pair<SchemeVertex*, WireType> connectedElement: vertex->getConnectedElements())
        {
            if(!connectedElement.first->isDistributed() && connectedElement.second == WireType::Inner)
                selectableElements.append(connectedElement.first);
        }
    }

    if(selectableElements.isEmpty())
        return undistributedVertices;

    return selectableElements;
}

QList<Scheme*> SerialDecomposition::buildSubschemes()
{
    QList<Scheme*> res;

    for(QList<SchemeVertex*>& list: distributedVertices)
    {
        Scheme* s = new Scheme();

        for(SchemeVertex* v: list)
            s->getElements().append(*(v->getElement()));

        for(Wire& w: scheme->getWires())
            s->getWires().append(w);

        res.append(s);
    }

    return res;
}
