#include "poolmanager.h"

PoolManager::PoolManager(int selfPort) :
    PoolEntity(selfPort)
{

}

void PoolManager::onNewConnection(QString, QHostAddress address, int tcpPort)
{
    transmitter->disconnectFromHost(address, tcpPort);
}

void PoolManager::connectToUnconnectedNodes()
{
    for(PoolNodeInfo& info: poolNodes)
    {
        if(info.getStatus() == NodeStatus::Unconnected)
        {
            if(tryConnect(info))
                info.setStatus(NodeStatus::Ready); // TODO: CALL IN MODEL
            else
                info.setStatus(NodeStatus::NotResponding);
        }
    }
}

bool PoolManager::tryConnect(PoolNodeInfo& info)
{
    try
    {
        transmitter->connectToHost(info.getAddress(), info.getTcpPort());
        return true;
    }
    catch(Exception&)
    {
        return false;
    }
}

void PoolManager::enableTransmitter()
{
    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection);
}

void PoolManager::disableTransmitter()
{
    if(transmitter == nullptr)
        return;

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection);
    PoolEntity::disableTransmitter();
}
