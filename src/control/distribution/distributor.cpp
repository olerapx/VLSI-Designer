#include "distributor.h"

Distributor::Distributor(Client& client) :
    client(client),
    initialLevel(0),
    currentLevel(0)
{

}

int Distributor::getLevelsNumber() const
{
    if(client.getArchitecture() == nullptr)
        return 0;

    return client.getArchitecture()->getModel().size() + 1;
}

int Distributor::getClientsNumberOnNextLevel(int levelNumber) const
{
    if(client.getArchitecture() == nullptr)
        return 0;

    if(levelNumber == getLevelsNumber() - 1)
        return 0;

    return client.getArchitecture()->getModel()[levelNumber];
}

bool Distributor::isLastLevel(int levelNumber) const
{
    if(levelNumber == getLevelsNumber() - 1)
        return true;

    return false;
}
