#include "leerouting.h"

LeeRouting::LeeRouting(Grid* grid, Scheme* scheme, PrimaryPlacementAlgorithm* algorithm, int maxExtensionAttempts) :
    RoutingAlgorithm(grid, scheme, algorithm, maxExtensionAttempts)
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

        if(isWireRouted(w))
            continue;

        if(w.getType() == WireType::Inner)
            innerWires.append(data);
        else
            outerWires.append(data);
    }

    std::sort(innerWires.begin(), innerWires.end(), WireDistanceComparator(gridHeight, gridWidth));
    std::sort(outerWires.begin(), outerWires.end(), WireDistanceComparator(gridHeight, gridWidth));
}

bool LeeRouting::isWireRouted(Wire& wire)
{
    qint64 index = wire.getIndex();
    for(RoutedWireIndex& i: grid->getRoutedWires())
    {
        if(i == index)
            return true;
    }

    return false;
}

void LeeRouting::routeWire(WireData data)
{

}

RoutingState LeeRouting::canRoute(QPoint from, QPoint to)
{
    CellInfo info = matrix[from.y()][from.x()];

    int diff = abs(to.x() - from.x()) + abs(to.y() - from.y());

    if(diff == 0)
        return { true, RoutingAction::Nothing, info.branched };

    if(diff != 1)
        throw RoutingException(tr("The given cells are not adjacent."));

    Direction direction;
    if(from.x() - to.x() == 1)
        direction = Direction::Left;
    else if(to.x() - from.x() == 1)
        direction = Direction::Right;
    else if(to.y() - from.y() == 1)
        direction = Direction::Up;
    else
        direction = Direction::Down;

    bool leave = canLeave(from, direction);
    bool enter = canEnter(to, !direction);

    if(leave)
    {
        if(!enter)
            return { false, RoutingAction::Nothing, true };

        if(info.branched)
            return { true, RoutingAction::Draw, true };

        return { true, RoutingAction::Branch, true };
    }

    if(enter)
        return { false, RoutingAction::WarnBrokenWire, true };

    if(info.branched)
        return { false, RoutingAction::Nothing, true };

    if(canLeave(to, !direction))
        return { false, RoutingAction::WarnBrokenWire, true };

    return { true, RoutingAction::Nothing, false };
}

