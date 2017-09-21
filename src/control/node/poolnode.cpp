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
        sendLog(tr("Got a new connection from %1:%2 but already have a connected pool manager."
                   "The connection was terminated.").arg(address.toString(), QString::number(tcpPort)));

        transmitter->disconnectFromHost(address, tcpPort);
        return;
    }

    sendLog(tr("Got a new connection from %1:%2.").arg(address.toString(), QString::number(tcpPort)));

    poolManager = new PoolManagerInfo(hostName, address, tcpPort);
}

void PoolNode::onDisconnected(QString, QHostAddress, int)
{
    delete poolManager;
    poolManager = nullptr;

    sendLog(tr("Lost connection with manager."));
}

void PoolNode::onDataReceived(QByteArray* /*data*/, QHostAddress /*address*/, int /*tcpPort*/)
{
    // TODO
}

void PoolNode::disconnectFromManager()
{
    transmitter->disconnectFromHost(poolManager->getAddress(), poolManager->getTcpPort());

    sendLog(tr("Disconnected from manager."));

    delete poolManager;
    poolManager = nullptr;
}

void PoolNode::enableTransmitter()
{
    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolNode::onDataReceived);
    connect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolNode::onDisconnected);

    sendLog(tr("Pool node is enabled."));
}

void PoolNode::disableTransmitter()
{
    if(transmitter == nullptr)
        return;

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
    disconnect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolNode::onDataReceived);
    disconnect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolNode::onDisconnected);
    PoolEntity::disableTransmitter();

    sendLog(tr("Pool node is disabled."));
}
