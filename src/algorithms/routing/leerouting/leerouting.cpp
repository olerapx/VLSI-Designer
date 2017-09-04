#include "leerouting.h"

LeeRouting::LeeRouting(Grid* grid, Scheme* scheme, int maxExtensionAttempts) :
    RoutingAlgorithm(grid, scheme, maxExtensionAttempts)
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

        sendLog(tr("Routing inner wires."));

        for(int i=0; i<innerWires.size(); i++)
        {
            sendLog(tr("Routing inner wire %1 of %2:")
                    .arg(QString::number(i+1), QString::number(innerWires.size())));
            routeWire(innerWires[i]);
        }

        sendLog(tr("Routing outer wires."));

        for(int i=0; i<outerWires.size(); i++)
        {
            sendLog(tr("Routing outer wire %1 of %2:")
                    .arg(QString::number(i+1), QString::number(outerWires.size())));
            routeWire(outerWires[i]);
        }

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

    startBranched = false;
}

void LeeRouting::initWires()
{
    if(stopped) return;
    sendLog(tr("Obtaining wires to be traced."));

    for(WireData& data: grid->getWiresData())
    {
        if(data.getWirePosition() == WirePosition::External)
            continue;

        qint64 index = data.getIndex();
        Wire& w = SchemeUtils::findWireByIndex(scheme, index);

        if(isWireRouted(w))
            continue;

        if(w.getType() == WireType::Inner)
            innerWires.append(&data);
        else
            outerWires.append(&data);
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

void LeeRouting::routeWire(WireData* data)
{
    do
    {
        if(stopped) return;

        initMatrix();
        getTerminalCoords(data);

        pushWave();

        if(noMoreWays)
        {
            if(extensionAttempts == maxExtensionAttempts)
                break;

            if(!tryExtend())
                break;

            extensionAttempts ++;
        }
    }
    while(!finished);

    if(!finished)
    {
        if(extensionAttempts == maxExtensionAttempts)
            sendLog(tr("Cannot route wire with index %1. Max extension attempts number is reached.").arg(QString::number(data->getIndex())));

        sendLog(tr("Cannot route wire with index %1. Grid extension is unavailable.").arg(QString::number(data->getIndex())));

        return;
    }

    pushReverseWave();

    grid->getRoutedWires().append(data->getIndex());
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

void LeeRouting::getTerminalCoords(WireData* data)
{
    startPinCoord = data->getSrcCoord();
    finishPinCoord = data->getDestCoord();

    std::pair<QPoint, bool> startPair = getNearbyAvailableCoord(startPinCoord);
    startCoord = startPair.first;
    startBranched = startPair.second;

    std::pair<QPoint, bool> finishPair = getNearbyAvailableCoord(finishPinCoord);
    finishCoord = finishPair.first;

    if(!finishPair.second)
        throw RoutingException(tr("Cannot route a wire to the pin (%1; %2). Multiple wires connected to the same pin which is prohibited.")
                               .arg(QString::number(finishPinCoord.x()), QString::number(finishPinCoord.y())));
}

std::pair<QPoint, bool> LeeRouting::getNearbyAvailableCoord(QPoint coord)
{
    QPoint points[] = { coord + QPoint(-1, 0), coord + QPoint(0, -1),
                        coord + QPoint(1, 0), coord + QPoint(0, 1) };

    Direction directions[] = { Direction::Right, Direction::Down, Direction::Left, Direction::Up };

    if(grid->getCells()[coord.y()][coord.x()].getType() != CellType::Pin)
        throw RoutingException(tr("Cannot start drawing wire from/to pin at coordinates(%1; %2): no pin found.")
                               .arg(QString::number(coord.x()), QString::number(coord.y())));

    for(int i=0; i<4; i++)
    {
        if(!checkCoordIsInGrid(points[i]))
            continue;

        if(grid->getCells()[points[i].y()][points[i].x()].getType() == CellType::Element)
        {
            QPoint diff = points[i] - coord;
            QPoint availableCoord = coord - diff;

            if(canLeave(availableCoord, !directions[i]))
                return std::make_pair(availableCoord, true);

            return std::make_pair(availableCoord, false);
        }
    }

    throw RoutingException(tr("Cannot start drawing wire from/to pin at coordinates(%1; %2): no element found nearby.")
                           .arg(QString::number(coord.x()), QString::number(coord.y())));
}

void LeeRouting::pushWave()
{
    sendLog(tr("Pushing the wave to find all possible routes."));

    currentValue = 0;
    matrix[startCoord.y()][startCoord.x()].value = currentValue;
    matrix[startCoord.y()][startCoord.x()].branched = startBranched;

    finished = false;
    noMoreWays = false;

    int pushedOnCurrentStep = 0;

    while(!finished && !noMoreWays)
    {
        if(stopped) return;

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
    if(!checkCoordIsInGrid(to))
        return false;

    qint64 value = matrix[to.y()][to.x()].value;

    if(value == -1 || value > currentValue + 1)
    {
        RoutingState state = canRoute(from, to, matrix[from.y()][from.x()].branched);

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
    sendLog(tr("Attempting to extend the grid."));

    QList<QPoint> lastWavePoints;

    for(int i=0; i<matrix.size(); i++)
    {
        for(int j=0; j<matrix[i].size(); j++)
        {
            if(matrix[i][j].value != -1)
                lastWavePoints.append(QPoint(j, i));
        }
    }

    std::sort(lastWavePoints.begin(), lastWavePoints.end(), PointDistanceComparator(finishCoord));

    QList<Direction> directions = { Direction::Left, Direction::Up, Direction::Right, Direction::Down };

    for(QPoint& point: lastWavePoints)
    {
        for(Direction& direction: directions)
        {
            if(extend(point, 1, direction))
            {
                gridHeight = grid->getCells().size();
                gridWidth = grid->getCells()[0].size();

                sendLog(tr("Successful extension."));
                return true;
            }
        }
    }

    sendLog(tr("Extension failed."));
    return false;
}

void LeeRouting::pushReverseWave()
{
    sendLog(tr("Drawing the most optimal route."));

    QPoint currentCoord = finishCoord;
    currentValue = matrix[currentCoord.y()][currentCoord.x()].value;

    Direction from, to;
    to = !getDirection(finishPinCoord, currentCoord);

    do
    {
        if(stopped) return;

        QPoint nearbyCoords[] = { currentCoord + QPoint(-1, 0), currentCoord + QPoint(0, -1),
                                  currentCoord + QPoint(1, 0), currentCoord + QPoint(0, 1) };

        bool hasProgress = false;

        for(int i=0; i<4; i++)
        {
            QPoint coord = nearbyCoords[i];

            if(!canAdvance(currentCoord, coord))
                continue;

            from = getDirection(currentCoord, coord);

            if(!draw(grid->getCells()[currentCoord.y()][currentCoord.x()], from, to))
                branch(grid->getCells()[currentCoord.y()][currentCoord.x()], to);

            to = !from;

            currentCoord = coord;
            currentValue --;

            hasProgress = true;

            break;
        }

        if(!hasProgress)
            throw RoutingException(tr("Cannot draw a wire from (%1; %2) to (%3; %4) but a possible way was found.")
                                   .arg(QString::number(startPinCoord.x()), QString::number(startPinCoord.y()),
                                        QString::number(finishPinCoord.x()), QString::number(finishPinCoord.y())));
    }
    while(currentCoord != startCoord);

    from = getDirection(currentCoord, startPinCoord);

    if(!draw(grid->getCells()[currentCoord.y()][currentCoord.x()], from, to))
        branch(grid->getCells()[currentCoord.y()][currentCoord.x()], to);
}

bool LeeRouting::canAdvance(QPoint currentCoord, QPoint coord)
{
    if(!checkCoordIsInGrid(coord))
        return false;

    if(matrix[coord.y()][coord.x()].value != currentValue - 1)
        return false;

    RoutingState state = canRoute(coord, currentCoord, matrix[coord.y()][coord.x()].branched);

    if(!state.canMove)
        return false;

    Direction from = getDirection(currentCoord, coord);

    bool currentBranched = matrix[currentCoord.y()][currentCoord.x()].branched;
    bool nextBranched = matrix[coord.y()][coord.x()].branched;

    if(!currentBranched && nextBranched)
        return false;

    if(!currentBranched && !nextBranched)
    {
        if(canLeave(currentCoord, from) || canLeave(coord, !from))
            return false;
    }

    return true;
}
