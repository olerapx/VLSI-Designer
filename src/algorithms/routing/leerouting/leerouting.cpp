#include "leerouting.h"

LeeRouting::LeeRouting(Grid* grid, Scheme* scheme) :
    RoutingAlgorithm(grid, scheme)
{
    clear();
}

Grid* LeeRouting::execute()
{
    try
    {
        if(!actuallyStopped)
            throw Exception(tr("The algorithm is already working."));

        stopped = false;
        actuallyStopped = false;

        gridHeight = grid->getCells().size();
        gridWidth = grid->getCells()[0].size();

        initMatrix();
        initWires();

        for(int i=0; i<innerWires.size(); i++)
            routeWire(innerWires[i]);

        for(int i=0; i<outerWires.size(); i++)
            routeWire(outerWires[i]);

        clear();

        if(stopped)
            throw ThreadStoppedException(tr("Algorithm is stopped."));

        stopped = true;
        actuallyStopped = true;

        return grid;
    }
    catch(...)
    {
        clear();

        stopped = true;
        actuallyStopped = true;

        throw;
    }
}

void LeeRouting::clear()
{
    innerWires.clear();
    outerWires.clear();

    for(QList<CellInfo>& list: matrix)
        list.clear();

    matrix.clear();

    gridHeight = 0;
    gridWidth = 0;
}

void LeeRouting::initMatrix()
{
    for(QList<Cell>& list: grid->getCells())
    {
        matrix.append(QList<CellInfo>());

        for(int i=0; i<list.size(); i++)
        {
            CellInfo info = { -1, true };
            matrix.last().append(info);
        }
    }
}

void LeeRouting::initWires()
{
    for(WireData& data: grid->getWiresData())
    {
        if(data.getWirePosition() == WirePosition::External)
            continue;

        qint64 index = data.getIndex();
        Wire& w = SchemeUtils::findWireByIndex(scheme, index);

        if(w.getType() == WireType::Inner)
            innerWires.append(data);
        else
            outerWires.append(data);
    }

    std::sort(innerWires.begin(), innerWires.end(), WireDistanceComparator(gridHeight, gridWidth));
    std::sort(outerWires.begin(), outerWires.end(), WireDistanceComparator(gridHeight, gridWidth));
}

void LeeRouting::routeWire(WireData data)
{

}
