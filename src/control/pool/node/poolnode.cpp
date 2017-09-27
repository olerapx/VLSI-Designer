#include "poolnode.h"

PoolNode::PoolNode(Version programVersion, int selfPort) :
    PoolEntity(selfPort),
    programVersion(programVersion),
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
    connect(&dispatcher, &CommandDispatcher::sendIdentify, this, &PoolNode::onIdentify, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendGetVersion, this, &PoolNode::onGetVersion, Qt::QueuedConnection);
}

void PoolNode::enable()
{
    if(transmitter != nullptr)
        return;

    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolNode::onDataReceived, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolNode::onDisconnected, Qt::QueuedConnection);

    sendLog(tr("Pool node is enabled."));
}

void PoolNode::disable()
{
    if(transmitter == nullptr)
        return;

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection);
    disconnect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolNode::onDataReceived);
    disconnect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolNode::onDisconnected);

    poolManager = nullptr;
    PoolEntity::disableTransmitter();

    sendLog(tr("Pool node is disabled."));
}

void PoolNode::onNewConnection(QHostAddress address, int tcpPort)
{
    sendLog(tr("Got a new connection from %1:%2, waiting for identification.").arg(address.toString(), QString::number(tcpPort)), LogType::Information);
    connectedEntities.append(PoolEntityInfo("", address, tcpPort));
}

void PoolNode::onDisconnected(QHostAddress address, int port)
{
    sendLog(tr("Lost connection with %1:%2.").arg(address.toString(), QString::number(port)), LogType::Warning);

    if(address == connectedEntities[0].getAddress() && port == connectedEntities[0].getTcpPort())
        poolManager = nullptr;

    connectedEntities.removeAll(PoolEntityInfo("", address, port));

    if(poolManager == nullptr)
    {
        sendLog(tr("Connection with manager was lost."), LogType::Warning);

        for(PoolEntityInfo& info: connectedEntities)
            transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
    }
}

void PoolNode::onDataReceived(QByteArray* data, QHostAddress address, int port)
{
    Command* command = new Command(data);

    if(dispatcher.isRequest(command->getType()))
        incomingRequests.append(CommandHistoryEntry(address, port, command->getType(), command->getUuid()));

    dispatcher.dispatchCommand(command);

    delete command;
}

void PoolNode::onIdentify(QUuid uuid, EntityType type)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    QString typeString = ((type == EntityType::Manager) ? tr("manager") : tr("node"));

    sendLog(tr("Connection from %1:%2 identified as %3.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort()), typeString));

    if(type == EntityType::Manager && poolManager != nullptr)
    {
        transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
        sendLog(tr("Connection with %1:%2 was aborted: already have a connected pool manager.")
                .arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

        connectedEntities.removeAll(info);

        return;
    }

    if(type == EntityType::Manager)
        poolManager = &(connectedEntities[0]);
}

void PoolNode::onGetVersion(QUuid uuid)
{
    QByteArray* body = new QByteArray();
    QDataStream stream(body, QIODevice::WriteOnly);

    stream << programVersion.toString();

    sendLog(tr("Sending response on program version request."), LogType::Information);

    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::SendVersion, uuid, body);
}
