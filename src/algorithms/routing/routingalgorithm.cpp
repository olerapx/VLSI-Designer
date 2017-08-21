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

RoutingAlgorithm::RoutingAlgorithm(Grid* grid, Scheme* scheme)
{
    setParameters(grid, scheme);
}

void RoutingAlgorithm::setParameters(Grid *grid, Scheme *scheme)
{
    this->grid = grid;
    this->scheme = scheme;
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

bool RoutingAlgorithm::canEnter(int x, int y, Direction direction)
{
    CellType type = grid->getCells()[y][x].getType();

    if(type == CellType::Element || type == CellType::Pin)
        return false;

    if(type == CellType::Empty)
    {
        if(hasElementNearby(x, y))
            return false;
        return true;
    }

    if(type == CellType::UD && (direction == Direction::Left || direction == Direction::Right))
        return true;

    if(type == CellType::LR && (direction == Direction::Up || direction == Direction::Down))
        return true;

    return false;
}

bool RoutingAlgorithm::hasElementNearby(int x, int y)
{
    int deltas[] = { -1, 0, 1 };

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(grid->getCells()[y + deltas[i]][x + deltas[j]].getType() == CellType::Element)
                return true;

    return false;
}

bool RoutingAlgorithm::canLeave(int x, int y, Direction direction)
{
    CellType type = grid->getCells()[y][x].getType();

    if(type == CellType::Element || type == CellType::Pin)
        return false;

    if(type == CellType::Empty)
        return true;

    if(type == CellType::UD && (direction == Direction::Left || direction == Direction::Right))
        return true;

    if(type == CellType::LR && (direction == Direction::Up || direction == Direction::Down))
        return true;

    if(type == CellType::UL && (direction == Direction::Down || direction == Direction::Right))
        return true;

    if(type == CellType::UR && (direction == Direction::Down || direction == Direction::Left))
        return true;

    if(type == CellType::DL && (direction == Direction::Up || direction == Direction::Right))
        return true;

    if(type == CellType::DR && (direction == Direction::Up || direction == Direction::Left))
        return true;

    if(type == CellType::UDL && direction == Direction::Right)
        return true;

    if(type == CellType::UDR && direction == Direction::Left)
        return true;

    if(type == CellType::LRU && direction == Direction::Down)
        return true;

    if(type == CellType::LRD && direction == Direction::Up)
        return true;

    return false;
}

void RoutingAlgorithm::draw(Cell& cell, Direction from, Direction to)
{    
    CellType newType = getDrawType(cell.getType(), from, to);
    cell.setType(newType);
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

void RoutingAlgorithm::branch(Cell& cell, Direction to)
{
    CellType newType = getBranchType(cell.getType(), to);
    cell.setType(newType);
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

RoutingState RoutingAlgorithm::canRoute(QPoint from, QPoint to)
{

}
