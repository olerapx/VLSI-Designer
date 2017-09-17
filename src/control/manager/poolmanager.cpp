#include "poolmanager.h"

PoolManager::PoolManager(int port) :
    port(port)
{
    usedInterface = nullptr;
}

void PoolManager::setUsedInterface(QNetworkInterface& interface)
{
    delete usedInterface;
    this->usedInterface = new QNetworkInterface(interface);
}

void PoolManager::setPort(int port)
{
    this->port = port; // TODO: add checking is manager working
}
