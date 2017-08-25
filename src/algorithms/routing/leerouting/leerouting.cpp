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

    matrix.clear();

    gridHeight = 0;
    gridWidth = 0;

    currentValue = 0;
    extensionAttempts = 0;

    finished = false;
    noMoreWays = false;
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

void LeeRouting::routeWire(WireData& data)
{
    extensionAttempts = 0;

    do
    {
        initMatrix();

        startPinCoord = data.getSrcCoord();
        finishPinCoord = data.getDestCoord();

        startCoord = getNearbyAvailableCoord(startPinCoord);
        finishCoord = getNearbyAvailableCoord(finishPinCoord);

        pushWave();
        if(noMoreWays)
        {
            if(!tryExtend())
                break;

            maxExtensionAttempts ++;
        }
    }
    while(!finished && extensionAttempts < maxExtensionAttempts);

    if(!finished && extensionAttempts == maxExtensionAttempts)
    {
        sendLog(tr("Cannot route wire with index %1. Max extension attempts number is reached.").arg(QString::number(data.getIndex())));
        return;
    }

    if(!finished)
    {
        sendLog(tr("Cannot route wire with index %1. Grid extension is unavailable.").arg(QString::number(data.getIndex())));
        return;
    }

    pushReverseWave();

    grid->getRoutedWires().append(data.getIndex());
}

void LeeRouting::initMatrix()
{
    matrix.clear();

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

QPoint LeeRouting::getNearbyAvailableCoord(QPoint coord)
{
    QPoint points[] = { QPoint(coord.x() - 1, coord.y()), QPoint(coord.x(), coord.y() - 1),
                      QPoint(coord.x() + 1, coord.y()), QPoint(coord.x(), coord.y() + 1) };

    Direction directions[] = { Direction::Right, Direction::Down, Direction::Left, Direction::Up };

    for(int i=0; i<4; i++)
    {
        if(canEnter(points[i], directions[i]))
            return points[i];
    }

    throw RoutingException(tr("Cannot start drawing wire from/to pin at coordinates(%1; %2): the pin is obscured.")
                           .arg(QString::number(coord.x()), QString::number(coord.y())));
}

void LeeRouting::pushWave()
{
    currentValue = 0;
    matrix[startCoord.y()][startCoord.x()].value = currentValue;
    matrix[startCoord.y()][startCoord.x()].branched = false;

    finished = false;
    noMoreWays = false;

    int pushedOnCurrentStep = 0;

    while(!finished && !noMoreWays)
    {
        pushedOnCurrentStep = 0;

        for(int i=0; i<matrix.size(); i++)
        {
            for(int j=0; j<matrix[i].size(); j++)
            {
                if(matrix[i][j].value != currentValue)
                    continue;

                QPoint currentCoord = QPoint(j, i);
                QPoint nearbyCoords[] = { QPoint(j - 1, i), QPoint(j, i - 1), QPoint(j + 1, i), QPoint(j, i + 1) };

                for(int k=0; k<4; k++)
                    if(tryRoute(currentCoord, nearbyCoords[k]))
                        pushedOnCurrentStep ++;
            }
        }

        if(pushedOnCurrentStep > 0)
            currentValue ++;
        else
            noMoreWays = true;
    }
}

bool LeeRouting::tryRoute(QPoint from, QPoint to)
{
    qint64 value = matrix[to.y()][to.x()].value;

    if(value == -1 || value > currentValue + 1)
    {
        RoutingState state = canRoute(from, to, matrix[from.y()][from.x()].branched);

        if(state.action == RoutingAction::WarnBrokenWire)
            sendLog(tr("A broken wire detected at (%1; %2).")
                    .arg(QString::number(from.x()), QString::number(from.y())));

        if(!state.canMove)
            return false;

        matrix[to.y()][to.x()].value = currentValue + 1;
        matrix[to.y()][to.x()].branched = state.newBranched;

        if(to == finishCoord)
            finished = true;

        return true;
    }

    return false;
}

bool LeeRouting::tryExtend()
{
    QList<QPoint> lastWavePoints;

    for(int i=0; i<matrix.size(); i++)
    {
        for(int j=0; j<matrix[i].size(); j++)
        {
            if(matrix[i][j].value == currentValue)
                lastWavePoints.append(QPoint(j, i));
        }
    }

    std::sort(lastWavePoints.begin(), lastWavePoints.end(), PointDistanceComparator(finishCoord));

    QList<Direction> directions = primaryPlacement->getPossibleExtensionDirections();

    for(QPoint& point: lastWavePoints)
    {
        for(Direction& direction: directions)
        {
            if(extend(point, 1, direction))
            {
                gridHeight = grid->getCells().size();
                gridWidth = grid->getCells()[0].size();

                return true;
            }
        }
    }

    return false;
}

void LeeRouting::pushReverseWave()
{
    QPoint currentCoord = finishCoord;

    Direction from, to;
    from = getDirection(finishPinCoord, currentCoord);

    do
    {
        QPoint nearbyCoords[] = { QPoint(currentCoord.x() - 1, currentCoord.y()), QPoint(currentCoord.x(), currentCoord.y() - 1),
                                  QPoint(currentCoord.x() + 1, currentCoord.y()), QPoint(currentCoord.x(), currentCoord.y() + 1) };

        for(int i=0; i<4; i++)
        {
           QPoint coord = nearbyCoords[i];

           if(matrix[coord.y()][coord.x()].value != currentValue - 1)
               continue;

           RoutingState state = canRoute(nearbyCoords[i], currentCoord, matrix[currentCoord.y()][currentCoord.x()].branched);

           if(!state.canMove)
               continue;

           to = getDirection(currentCoord, coord);

           if(state.action == RoutingAction::Draw)
           {
                draw(grid->getCells()[currentCoord.y()][currentCoord.x()], !from, to);
           }
           else if(state.action == RoutingAction::Branch)
           {
                branch(grid->getCells()[currentCoord.y()][currentCoord.x()], to);
           }

           from = to;

           currentCoord = coord;

           break;
        }
    }
    while(currentCoord != startCoord);

    to = getDirection(currentCoord, startPinCoord);
    draw(grid->getCells()[currentCoord.y()][currentCoord.x()], !from, to);
}
