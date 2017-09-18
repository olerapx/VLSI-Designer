#include "poolmanager.h"

PoolManager::PoolManager(int port) :
    port(port)
{

}

void PoolManager::setPort(int port)
{
    this->port = port; // TODO: add checking is manager working
}
