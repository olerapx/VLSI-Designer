#include "defaultdistributor.h"

DefaultDistributor::DefaultDistributor(Client& client) :
    Distributor(client)
{

}

void DefaultDistributor::start(Scheme* scheme, int initialLevel)
{
    this->initialLevel = initialLevel;
}

void DefaultDistributor::onIncomingGrid(Grid* grid, int level)
{

}

void DefaultDistributor::onError(QString)
{

}

void DefaultDistributor::onReceivedNodes()
{

}
