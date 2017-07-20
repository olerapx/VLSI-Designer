#include "serialdecomposition.h"

SerialDecomposition::SerialDecomposition(Scheme* scheme, int number):
    DecompositionAlgorithm(scheme, number)
{
    clear();
}

QList<Scheme*> SerialDecomposition::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        fillVertices();
        prepareSubschemes();

        sendLog(tr("Elements decomposition."));
        int i = 1;
        for(QList<SchemeVertex*>& list: distributedVertices)
        {
            sendLog(tr("Filling subscheme %1 of %2:").arg(QString::number(i++), QString::number(distributedVertices.size())));
            fillSubscheme(list);
        }

        sendLog(tr("Finishing."));
        QList<Scheme*> list = buildSubschemes();
        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

        stopped = true;
        actuallyStopped = true;

        return list;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
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
    if(stopped) return;

    sendLog(tr("Preparing."));    

    for(SchemeElement& el: scheme->getElements())
        undistributedVertices.append(new SchemeVertex(&el));

    for(Wire& w: scheme->getWires())
    {
        if(stopped) return;

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
    if(stopped) return;

    sendLog(tr("Creating empty subschemes."));

    qint64 quotient = scheme->getElements().size() / number;
    qint64 reminder = scheme->getElements().size() % number;

    for(int i=0; i < number; i++)
    {
        QList<SchemeVertex*> list;
        for(qint64 j=0; j<quotient; j++)
            list.append(nullptr);

        distributedVertices.append(list);
    }

    for(int i=0; i<reminder; i++)
        distributedVertices[i].append(nullptr);
}

void SerialDecomposition::fillSubscheme(QList<SchemeVertex*>& list)
{
    if(stopped) return;
    placeFirstElement(list);

    for(int i=1; i<list.size(); i++)
    {
        if(stopped) return;

        placeNextElement(list, i);
    }
}

void SerialDecomposition::placeFirstElement(QList<SchemeVertex*> &list)
{
    sendLog(tr("First element placement."));
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
    sendLog(tr("Placement of element %1 of %2.").arg(QString::number(index + 1), QString::number(list.size())));

    QList<SchemeVertex*> selectableVertices = getSelectableVertices(list);
    std::sort(selectableVertices.begin(), selectableVertices.end(), OuterConnectionsNumberComparator(list));

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
        if(stopped) return res;

        Scheme* s = new Scheme();

        for(SchemeVertex* v: list)
            s->getElements().append(*(v->getElement()));

        for(Wire& w: scheme->getWires())
            s->getWires().append(w);

        res.append(s);
    }

    return res;
}
