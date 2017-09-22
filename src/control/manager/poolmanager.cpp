#include "poolmanager.h"

PoolManager::PoolManager(int selfPort) :
    PoolEntity(selfPort)
{

}

void PoolManager::enable()
{
    if(transmitter != nullptr)
        return;

    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolManager::onDataReceived);
    connect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolManager::onDisconnected);

    sendLog(tr("Pool manager is enabled."));
}

void PoolManager::disable()
{
    if(transmitter == nullptr)
        return;

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection);
    disconnect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolManager::onDataReceived);
    disconnect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolManager::onDisconnected);
    PoolEntity::disableTransmitter();

    sendLog(tr("Pool manager is disabled."));
}

void PoolManager::connectToUnconnectedNodes()
{
    for(PoolNodeInfo& info: poolNodes)
        connectToNode(info);
}

void PoolManager::connectToNode(PoolNodeInfo &info)
{
    if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
        return;

    sendLog(tr("Connecting to %1:%2...").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    info.setStatus(NodeStatus::Connecting);
    sendUpdateNodeInfo(info);

    if(tryConnect(info))
    {
        info.setStatus(NodeStatus::Ready);
        sendLog(tr("Success."));
    }
    else
    {
        info.setStatus(NodeStatus::NotResponding);
        sendLog(tr("Connection failed."));
    }

    sendUpdateNodeInfo(info);
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

void PoolManager::removeNode(PoolNodeInfo& info)
{
    if(info.getStatus() == NodeStatus::Unconnected || info.getStatus() == NodeStatus::NotResponding)
        return;

    disconnectFromNode(info);

    int index = poolNodes.indexOf(info);
    poolNodes.removeAt(index);
    sendRemoveNodeInfo(index);
}

void PoolManager::disconnectFromNode(PoolNodeInfo& info)
{
    sendLog(tr("Disconnecting from %1:%2...").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());

    info.setStatus(NodeStatus::Unconnected);
    sendUpdateNodeInfo(info);
}

PoolNodeInfo& PoolManager::getInfoByAddressAndPort(QHostAddress address, int port)
{
    for(PoolNodeInfo& info: poolNodes)
    {
        if(info.getAddress() == address && info.getTcpPort() == port)
            return info;
    }

    throw IllegalArgumentException(tr("Trying to access pool node at %1:%2 but node existed.")
                                   .arg(address.toString(), QString::number(port)));
}

void PoolManager::onNewConnection(QString, QHostAddress address, int tcpPort)
{
    transmitter->disconnectFromHost(address, tcpPort);

    sendLog(tr("Got an unexpected new connection from %1:%2. The connection was terminated.")
            .arg(address.toString(), QString::number(tcpPort)));
}

void PoolManager::onDisconnected(QString, QHostAddress address, int tcpPort)
{
    PoolNodeInfo& info = getInfoByAddressAndPort(address, tcpPort);

    info.setStatus(NodeStatus::NotResponding);
    sendUpdateNodeInfo(info);

    sendLog(tr("Lost connection with node at %1:%2.")
            .arg(address.toString(), QString::number(tcpPort)));
}

void PoolManager::onDataReceived(QByteArray* /*data*/, QHostAddress /*address*/, int /*tcpPort*/)
{
    // TODO
}
