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

    startBranched = false;
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
        initMatrix();

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

    if(!finished && extensionAttempts == maxExtensionAttempts)
    {
        sendLog(tr("Cannot route wire with index %1. Max extension attempts number is reached.").arg(QString::number(data->getIndex())));
        return;
    }

    if(!finished)
    {
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

std::pair<QPoint, bool> LeeRouting::getNearbyAvailableCoord(QPoint coord)
{
    QPoint points[] = { QPoint(coord.x() - 1, coord.y()), QPoint(coord.x(), coord.y() - 1),
                      QPoint(coord.x() + 1, coord.y()), QPoint(coord.x(), coord.y() + 1) };

    Direction directions[] = { Direction::Right, Direction::Down, Direction::Left, Direction::Up };

    if(grid->getCells()[coord.y()][coord.x()].getType() != CellType::Pin)
        throw RoutingException(tr("Cannot start drawing wire from/to pin at coordinates(%1; %2): no pin found.")
                               .arg(QString::number(coord.x()), QString::number(coord.y())));

    for(int i=0; i<4; i++)
    {
        if(!validateCoord(points[i]))
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
    currentValue = 0;
    matrix[startCoord.y()][startCoord.x()].value = currentValue;
    matrix[startCoord.y()][startCoord.x()].branched = startBranched;

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
    if(!validateCoord(to))
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

                return true;
            }
        }
    }

    return false;
}

void LeeRouting::pushReverseWave()
{
    QPoint currentCoord = finishCoord;
    currentValue = matrix[currentCoord.y()][currentCoord.x()].value;

    Direction from, to;
    to = !getDirection(finishPinCoord, currentCoord);

    bool currentBranched = matrix[currentCoord.y()][currentCoord.x()].branched;
    bool routed = false;

    do
    {
        QPoint nearbyCoords[] = { QPoint(currentCoord.x() - 1, currentCoord.y()), QPoint(currentCoord.x(), currentCoord.y() - 1),
                                  QPoint(currentCoord.x() + 1, currentCoord.y()), QPoint(currentCoord.x(), currentCoord.y() + 1) };

        routed = false;

        for(int i=0; i<4; i++)
        {
           QPoint coord = nearbyCoords[i];

           if(!validateCoord(coord))
               continue;

           if(matrix[coord.y()][coord.x()].value != currentValue - 1)
               continue;

           bool nextBranched = matrix[coord.y()][coord.x()].branched;

           if(!currentBranched && nextBranched)
               continue;

           RoutingState state = canRoute(nearbyCoords[i], currentCoord, matrix[nearbyCoords[i].y()][nearbyCoords[i].x()].branched);

           if(!state.canMove)
               continue;

           from = getDirection(currentCoord, coord);

           if(!draw(grid->getCells()[currentCoord.y()][currentCoord.x()], from, to))
               branch(grid->getCells()[currentCoord.y()][currentCoord.x()], to);

           to = !from;

           currentCoord = coord;
           currentValue --;

           currentBranched = nextBranched;

           routed = true;

           break;
        }

        if(!routed)
            throw RoutingException(tr("Cannot draw a wire from (%1; %2) to (%3; %4). "
                                      "Perhaps the scheme has multiple outputs connected to the same input which is forbidden.")
                                   .arg(QString::number(startPinCoord.x()), QString::number(startPinCoord.y()),
                                        QString::number(finishPinCoord.x()), QString::number(finishPinCoord.y())));
    }
    while(currentCoord != startCoord);

    from = getDirection(currentCoord, startPinCoord);

    if(!draw(grid->getCells()[currentCoord.y()][currentCoord.x()], from, to))
        branch(grid->getCells()[currentCoord.y()][currentCoord.x()], to);
}
