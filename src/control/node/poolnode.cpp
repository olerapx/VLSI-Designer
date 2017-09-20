#include "poolnode.h"

PoolNode::PoolNode(int selfPort) :
    PoolEntity(selfPort),
    poolManager(nullptr)
{

}

PoolNode::~PoolNode()
{
    delete poolManager;
}

void PoolNode::onNewConnection(QString hostName, QHostAddress address, int tcpPort)
{
    if(poolManager != nullptr)
    {
        transmitter->disconnectFromHost(address, tcpPort);
        return;
    }

    poolManager = new PoolManagerInfo(hostName, address, tcpPort);
}

void PoolNode::disconnectFromManager()
{
    transmitter->disconnectFromHost(poolManager->getAddress(), poolManager->getTcpPort());

    delete poolManager;
    poolManager = nullptr;
}

void PoolNode::enableTransmitter()
{
    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
}

void PoolNode::disableTransmitter()
{
    if(transmitter == nullptr)
        return;

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
    PoolEntity::disableTransmitter();
}
