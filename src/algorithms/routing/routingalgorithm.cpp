#include "routingalgorithm.h"

Direction operator !(const Direction& other)
{
    switch(other)
    {
        case Direction::Left:
            return Direction::Right;
        case Direction::Right:
            return Direction::Left;
        case Direction::Up:
            return Direction::Down;
        default:
            return Direction::Up;
    }
}

RoutingAlgorithm::RoutingAlgorithm(Grid* grid, Scheme* scheme, PrimaryPlacementAlgorithm* algorithm, int maxExtensionAttempts)
{
    setParameters(grid, scheme, algorithm, maxExtensionAttempts);
}

void RoutingAlgorithm::setParameters(Grid* grid, Scheme* scheme, PrimaryPlacementAlgorithm* algorithm, int maxExtensionAttempts)
{
    this->grid = grid;
    this->scheme = scheme;
    this->primaryPlacement = algorithm;
    this->maxExtensionAttempts = maxExtensionAttempts;
}

void RoutingAlgorithm::onStart()
{
    try
    {
        Grid* grid = execute();
        sendResult(grid);
        sendFinish();
    }
    catch(ThreadStoppedException&)
    {
        sendFinish();
    }
    catch(Exception& e)
    {
        sendError(e.what());
        sendFinish();
    }
}

bool RoutingAlgorithm::canEnter(QPoint coord, Direction from)
{
    if(!validateCoord(coord))
        return false;

    CellType type = grid->getCells()[coord.y()][coord.x()].getType();

    if(type == CellType::Element || type == CellType::Pin)
        return false;

    if(type == CellType::Empty)
    {
        if(hasElementNearby(coord))
            return false;
        return true;
    }

    if(type == CellType::UD && (from == Direction::Left || from == Direction::Right))
        return true;

    if(type == CellType::LR && (from == Direction::Up || from == Direction::Down))
        return true;

    return false;
}

bool RoutingAlgorithm::validateCoord(QPoint coord)
{
    if(coord.y() < 0 || coord.y() >= grid->getCells().size() ||
            coord.x() < 0 || coord.x() >= grid->getCells()[coord.y()].size())
        return false;

    return true;
}

bool RoutingAlgorithm::hasElementNearby(QPoint coord)
{
    int deltas[] = { -1, 0, 1 };

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            QPoint nearbyCoord = QPoint(coord.x() + deltas[j], coord.y() + deltas[i]);
            if(!validateCoord(nearbyCoord))
                continue;

            if(grid->getCells()[nearbyCoord.y()][nearbyCoord.x()].getType() == CellType::Element)
                return true;
        }

    return false;
}

bool RoutingAlgorithm::canLeave(QPoint coord, Direction to)
{
    if(!validateCoord(coord))
        return false;

    CellType type = grid->getCells()[coord.y()][coord.x()].getType();

    if(type == CellType::Element || type == CellType::Pin)
        return false;

    if(type == CellType::Empty)
        return true;

    if(type == CellType::UD && (to == Direction::Left || to == Direction::Right))
        return true;

    if(type == CellType::LR && (to == Direction::Up || to == Direction::Down))
        return true;

    if(type == CellType::UL && (to == Direction::Down || to == Direction::Right))
        return true;

    if(type == CellType::UR && (to == Direction::Down || to == Direction::Left))
        return true;

    if(type == CellType::DL && (to == Direction::Up || to == Direction::Right))
        return true;

    if(type == CellType::DR && (to == Direction::Up || to == Direction::Left))
        return true;

    if(type == CellType::UDL && to == Direction::Right)
        return true;

    if(type == CellType::UDR && to == Direction::Left)
        return true;

    if(type == CellType::LRU && to == Direction::Down)
        return true;

    if(type == CellType::LRD && to == Direction::Up)
        return true;

    return false;
}

bool RoutingAlgorithm::draw(Cell& cell, Direction from, Direction to)
{    
    try
    {
        CellType newType = getDrawType(cell.getType(), from, to);
        cell.setType(newType);
    }
    catch(RoutingException& e)
    {
        sendLog(e.what());

        return false;
    }

    return true;
}

CellType RoutingAlgorithm::getDrawType(CellType type, Direction from, Direction to)
{
    if(from == to)
        return type;

    if(from > to)
        std::swap(from, to);

    if(from == Direction::Left)
    {
        if(to == Direction::Up && type == CellType::Empty)
            return CellType::UL;

        if(to == Direction::Right)
        {
            if(type == CellType::Empty)
                return CellType::LR;
            if(type == CellType::UD)
                return CellType::UDLR;
        }

        if(to == Direction::Down && type == CellType::Empty)
            return CellType::DL;
    }

    if(from == Direction::Up)
    {
        if(to == Direction::Right && type == CellType::Empty)
            return CellType::UR;

        if(to == Direction::Down)
        {
            if(type == CellType::Empty)
                return CellType::UD;
            if(type == CellType::LR)
                return CellType::UDLR;
        }
    }

    if(from == Direction::Right && to == Direction::Down && type == CellType::Empty)
        return CellType::DR;


    throw RoutingException(tr("Cannot draw a wire with the given directions: either the directions are wrong or the cell type is incorrect."));
}

bool RoutingAlgorithm::branch(Cell& cell, Direction to)
{
    try
    {
        CellType newType = getBranchType(cell.getType(), to);
        cell.setType(newType);
    }
    catch(RoutingException& e)
    {
        sendLog(e.what());

        return false;
    }

    return true;
}

CellType RoutingAlgorithm::getBranchType(CellType type, Direction to)
{
    if(type == CellType::Empty || type == CellType::Element || type == CellType::Pin ||
            type == CellType::UDLR || type == CellType::UDLRI)
        throw RoutingException(tr("Cannot branch a wire on the cell without wire already existed."));

    if(type == CellType::UDL && to == Direction::Right)
        return CellType::UDLRI;

    if(type == CellType::UDR && to == Direction::Left)
        return CellType::UDLRI;

    if(type == CellType::LRU && to == Direction::Down)
        return CellType::UDLRI;

    if(type == CellType::LRD && to == Direction::Up)
        return CellType::UDLRI;

    if(type == CellType::UD)
    {
        if(to == Direction::Left)
            return CellType::UDL;
        if(to == Direction::Right)
            return CellType::UDR;
    }

    if(type == CellType::LR)
    {
        if(to == Direction::Up)
            return CellType::LRU;
        if(to == Direction::Down)
            return CellType::LRD;
    }

    if(type == CellType::UL)
    {
        if(to == Direction::Right)
            return CellType::LRU;
        if(to == Direction::Down)
            return CellType::UDL;
    }

    if(type == CellType::UR)
    {
        if(to == Direction::Down)
            return CellType::UDR;
        if(to == Direction::Left)
            return CellType::LRU;
    }

    if(type == CellType::DL)
    {
        if(to == Direction::Up)
            return CellType::UDL;
        if(to == Direction::Right)
            return CellType::LRD;
    }

    if(type == CellType::DR)
    {
        if(to == Direction::Up)
            return CellType::UDR;
        if(to == Direction::Left)
            return CellType::LRD;
    }

    throw RoutingException(tr("Incorrect combination of wire type and direction is specified."));
}

RoutingState RoutingAlgorithm::canRoute(QPoint from, QPoint to, bool branched)
{
    if(!validateCoord(from) || !validateCoord(to))
        return { false, true };

    CellType type = grid->getCells()[to.y()][to.x()].getType();
    if(type == CellType::Pin || type == CellType::Element)
        return { false, true };

    int diff = abs(to.x() - from.x()) + abs(to.y() - from.y());

    if(diff == 0)
        return { true, branched };

    if(diff != 1)
        throw RoutingException(tr("The given cells are not adjacent."));

    Direction direction = getDirection(from, to);

    bool leave = canLeave(from, direction);
    bool enter = canEnter(to, !direction);

    if(leave)
    {
        if(!enter)
            return { false, true };

        return { true, true };
    }

    if(enter)
    {
        sendLog(tr("A broken wire detected at (%1; %2).")
                .arg(QString::number(from.x()), QString::number(from.y())));

        return { false, true };
    }

    if(branched)
        return { false, true };

    if(canLeave(to, !direction))
    {
        sendLog(tr("A broken wire detected at (%1; %2).")
                .arg(QString::number(from.x()), QString::number(from.y())));

        return { false, true };
    }

    return { true, false };
}

Direction RoutingAlgorithm::getDirection(QPoint from, QPoint to)
{
    Direction direction;
    if(from.x() - to.x() == 1)
        direction = Direction::Left;
    else if(to.x() - from.x() == 1)
        direction = Direction::Right;
    else if(from.y() - to.y() == 1)
        direction = Direction::Up;
    else
        direction = Direction::Down;

    return direction;
}

bool RoutingAlgorithm::extend(QPoint coord, int number, Direction direction)
{
    if(!validateCoord(coord))
        return false;

    if(direction == Direction::Left || direction == Direction::Right)
        return extendHorizontally(coord, number, direction);

    return extendVertically(coord, number, direction);
}

bool RoutingAlgorithm::extendHorizontally(QPoint coord, int number, Direction direction)
{
    for(int i=0; i<grid->getCells().size(); i++)
    {
        CellType type = grid->getCells()[i][coord.x()].getType();

        if(type == CellType::Element || type == CellType::Pin)
            return false;
    }

    int insertX = 0;

    if(direction == Direction::Left)
        insertX = coord.x();
    else
        insertX = coord.x() + 1;

    for(int i=0; i<grid->getCells().size(); i++)
    {
        CellType type = CellType::Empty;

        if((grid->getCells()[i][coord.x()].isWire() && !canLeave(QPoint(coord.x(), i), direction)) ||
                (direction == Direction::Left && coord.x() - 1 >= 0 && grid->getCells()[i][coord.x() - 1].isWire() &&
                 !canLeave(QPoint(coord.x() - 1, i), !direction)) ||
                (direction == Direction::Right && coord.x() + 1 < grid->getCells()[i].size() && grid->getCells()[i][coord.x() + 1].isWire() &&
                 !canLeave(QPoint(coord.x() + 1, i), !direction)))
            type = CellType::LR;

        for(int j=0; j<number; j++)
        {
            grid->getCells()[i].insert(insertX, Cell(type));
        }
    }

    for(WireData& data: grid->getWiresData())
    {
        if(data.getSrcCoord().x() >= insertX)
            data.setSrcCoord(data.getSrcCoord() + QPoint(number, 0));

        if(data.getDestCoord().x() >= insertX)
            data.setDestCoord(data.getDestCoord() + QPoint(number, 0));
    }

    return true;
}

bool RoutingAlgorithm::extendVertically(QPoint coord, int number, Direction direction)
{
    for(int j=0; j<grid->getCells()[coord.y()].size(); j++)
    {
        CellType type = grid->getCells()[coord.y()][j].getType();

        if(type == CellType::Element || type == CellType::Pin)
            return false;
    }

    int insertY = 0;

    if(direction == Direction::Up)
        insertY = coord.y();
    else
        insertY = coord.y() + 1;

    QList<QList<Cell>> newCells;
    for(int i=0; i<number; i++)
        newCells.append(QList<Cell>());

    for(int j=0; j<grid->getCells()[coord.y()].size(); j++)
    {
        CellType type = CellType::Empty;

        if((grid->getCells()[coord.y()][j].isWire() && !canLeave(QPoint(j, coord.y()), direction)) ||
                (direction == Direction::Up && coord.y() - 1 >= 0 && grid->getCells()[coord.y() - 1][j].isWire() &&
                 !canLeave(QPoint(j, coord.y() - 1), !direction)) ||
                (direction == Direction::Down && coord.y() + 1 < grid->getCells().size() && grid->getCells()[coord.y() + 1][j].isWire() &&
                 !canLeave(QPoint(j, coord.y() + 1), !direction)))
            type = CellType::UD;

        for(int i=0; i<number; i++)
        {
            newCells[i].append(Cell(type));
        }
    }

    for(int i=0; i<number; i++)
    {
        grid->getCells().insert(insertY, newCells[i]);
    }

    for(WireData& data: grid->getWiresData())
    {
        if(data.getSrcCoord().y() >= insertY)
            data.setSrcCoord(data.getSrcCoord() + QPoint(0, number));

        if(data.getDestCoord().y() >= insertY)
            data.setDestCoord(data.getDestCoord() + QPoint(0, number));
    }

    return true;
}
