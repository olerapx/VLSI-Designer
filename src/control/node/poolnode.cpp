#include "poolnode.h"

PoolNode::PoolNode(int selfPort) :
    PoolEntity(selfPort),
    poolManager(nullptr)
{
    connectDispatcher();
}

PoolNode::~PoolNode()
{
    disable();
}

void PoolNode::connectDispatcher()
{
    connect(&dispatcher, &CommandDispatcher::sendGetVersion, this, &PoolNode::onGetVersion);
}

void PoolNode::enable()
{
    if(transmitter != nullptr)
        return;

    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolNode::onDataReceived);
    connect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolNode::onDisconnected);

    sendLog(tr("Pool node is enabled."));
}

void PoolNode::disable()
{
    if(transmitter == nullptr)
        return;

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
    disconnect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolNode::onDataReceived);
    disconnect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolNode::onDisconnected);

    if(poolManager != nullptr)
        delete poolManager;

    PoolEntity::disableTransmitter();

    sendLog(tr("Pool node is disabled."));
}

void PoolNode::disconnectFromManager()
{
    transmitter->disconnectFromHost(poolManager->getAddress(), poolManager->getTcpPort());

    sendLog(tr("Disconnected from manager."));

    delete poolManager;
    poolManager = nullptr;
}

void PoolNode::sendCommand(CommandType type, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, body);
    transmitter->sendData(command->toByteArray(), poolManager->getAddress(), poolManager->getTcpPort());

    delete command;
}

void PoolNode::sendResponse(CommandType type, QUuid uuid, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, uuid, body);
    transmitter->sendData(command->toByteArray(), poolManager->getAddress(), poolManager->getTcpPort());

    delete command;
}

void PoolNode::onNewConnection(QHostAddress address, int tcpPort)
{
    if(poolManager != nullptr)
    {
        sendLog(tr("Got a new connection from %1:%2 but already have a connected pool manager."
                   "The connection was terminated.").arg(address.toString(), QString::number(tcpPort)));

        transmitter->disconnectFromHost(address, tcpPort);
        return;
    }

    sendLog(tr("Got a new connection from %1:%2.").arg(address.toString(), QString::number(tcpPort)));

    poolManager = new PoolManagerInfo(address, tcpPort);
}

void PoolNode::onDisconnected(QString, QHostAddress, int)
{
    delete poolManager;
    poolManager = nullptr;

    sendLog(tr("Lost connection with manager."));
}

void PoolNode::onDataReceived(QByteArray* data, QHostAddress, int)
{
    Command* command = new Command(data);
    dispatcher.dispatchCommand(command);

    delete command;
}

void PoolNode::onGetVersion(QUuid uuid)
{
    QByteArray* body = new QByteArray();
    QDataStream stream(body, QIODevice::WriteOnly);

    double d = 1.3;
    stream << d; // TODO

    sendLog(tr("Sending response on program version request."));

    sendResponse(CommandType::SendVersion, uuid, body);
}
