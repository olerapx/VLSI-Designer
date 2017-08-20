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

CellType RoutingAlgorithm::draw(CellType type, Direction from, Direction to)
{
    ///TODO
}

CellType RoutingAlgorithm::branch(CellType type, Direction to)
{
    ///TODO
}
