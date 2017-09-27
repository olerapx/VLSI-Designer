#include "poolmanager.h"

PoolManager::PoolManager(Version programVersion, int selfPort) :
    PoolEntity(selfPort),
    started(false),
    programVersion(programVersion)
{
    data = new SessionData();
    connectDispatcher();
}

PoolManager::~PoolManager()
{
    disable();
    delete data;
}

void PoolManager::connectDispatcher()
{
    connect(&dispatcher, &CommandDispatcher::sendSendVersion, this, &PoolManager::onSendVersion, Qt::QueuedConnection);
}

void PoolManager::enable()
{
    if(transmitter != nullptr)
        return;

    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolManager::onDataReceived, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolManager::onDisconnected, Qt::QueuedConnection);

    sendLog(tr("Pool manager is enabled."));
}

void PoolManager::disable()
{
    if(transmitter == nullptr)
        return;

    started = false;

    sendClearNodesInfo();

    disconnect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection);
    disconnect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolManager::onDataReceived);
    disconnect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolManager::onDisconnected);

    PoolEntity::disableTransmitter();

    sendLog(tr("Pool manager is disabled."));
}

void PoolManager::connectToUnconnectedNodes()
{
    for(PoolEntityInfo& info: connectedEntities)
        connectToNode(info);
}

void PoolManager::connectToNode(PoolEntityInfo &info)
{
    if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
        return;

    sendLog(tr("Connecting to %1:%2...").arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Information);

    info.setStatus(NodeStatus::Connecting);
    sendUpdateNodeInfo(info);

    if(tryConnect(info))
    {
        info.setStatus(NodeStatus::Ready);
        sendUpdateNodeInfo(info);
        sendLog(tr("Success."), LogType::Success);

        QByteArray* body = new QByteArray();
        QDataStream stream(body, QIODevice::WriteOnly);
        stream << (qint32) entityType;

        sendUntrackedRequest(info.getAddress(), info.getTcpPort(), CommandType::Identify, body);

        sendLog(tr("Requesting for program version..."), LogType::Information);
        sendRequest(info.getAddress(), info.getTcpPort(), CommandType::GetVersion);
    }
    else
    {
        info.setStatus(NodeStatus::NotResponding);
        sendUpdateNodeInfo(info);
        sendLog(tr("Connection failed."), LogType::Warning);
    }
}

bool PoolManager::tryConnect(PoolEntityInfo& info)
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

void PoolManager::removeNode(PoolEntityInfo& info)
{
    if(info.getStatus() == NodeStatus::Unconnected || info.getStatus() == NodeStatus::NotResponding)
    {
        int index = connectedEntities.indexOf(info);
        connectedEntities.removeAt(index);
        sendRemoveNodeInfo(index);

        return;
    }

    QObject* obj = new QObject(this);
    QObject::connect(this, &PoolManager::sendDisconnected, obj, [this, obj, &info](QHostAddress address, int tcpPort)
    {
        if(address == info.getAddress() && tcpPort == info.getTcpPort())
        {
            int index = connectedEntities.indexOf(info);

            connectedEntities.removeAt(index);
            sendRemoveNodeInfo(index);

            obj->deleteLater();
        }
    });

    disconnectFromNodeWithoutNotification(info);
}

void PoolManager::disconnectFromNode(PoolEntityInfo& info)
{
    QObject* obj = new QObject(this);

    QObject::connect(this, &PoolManager::sendDisconnected, obj, [this, obj, &info](QHostAddress address, int tcpPort)
    {
        if(address == info.getAddress() && tcpPort == info.getTcpPort())
        {
            info.setStatus(NodeStatus::Unconnected);
            sendUpdateNodeInfo(info);

            obj->deleteLater();
        }
    });

    disconnectFromNodeWithoutNotification(info);
}

void PoolManager::disconnectFromNodeWithoutNotification(PoolEntityInfo &info)
{
    sendLog(tr("Disconnecting from %1:%2...").arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Information);
    transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
}

void PoolManager::setSessionData(SessionData* data)
{
    delete this->data;
    this->data = data;
}

void PoolManager::start()
{
    started = true;

    setStatusOfAllConnectedNodes(NodeStatus::Initialization);
}

void PoolManager::setStatusOfAllConnectedNodes(NodeStatus status)
{
    for(PoolEntityInfo& info: connectedEntities)
    {
        if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
        {
            info.setStatus(status);
            sendUpdateNodeInfo(info);
        }
    }
}

void PoolManager::onNewConnection(QHostAddress address, int tcpPort)
{
    transmitter->disconnectFromHost(address, tcpPort);
    sendLog(tr("Got an unexpected new connection from %1:%2. The connection was terminated.").arg(address.toString(), QString::number(tcpPort)));
}

void PoolManager::onDisconnected(QHostAddress address, int tcpPort)
{
    PoolEntityInfo& info = getInfoByAddressAndPort(address, tcpPort);

    info.setStatus(NodeStatus::NotResponding);
    sendUpdateNodeInfo(info);

    sendLog(tr("Lost connection with node at %1:%2.").arg(address.toString(), QString::number(tcpPort)), LogType::Warning);

    sendDisconnected(address, tcpPort);
}

void PoolManager::onDataReceived(QByteArray* data, QHostAddress, int)
{
    Command* command = new Command(data);

    if(!dispatcher.isRequest(command->getType()))
        dispatcher.dispatchCommand(command);

    delete command;
}

void PoolManager::onSendVersion(QUuid uuid, Version version)
{
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);

    sendLog(tr("Received a program version from %1:%2.").arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Information);

    info.setProgramVersion(version);
    sendUpdateNodeInfo(info);

    if(info.getProgramVersion() == programVersion)
    {
        sendLog(tr("Version is compatible."), LogType::Success);
        return;
    }

    sendLog(tr("The node %1:%2 has an incompatible program version, disconnecting.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Warning);

    QObject* obj = new QObject(this);

    QObject::connect(this, &PoolManager::sendDisconnected, obj, [this, obj, &info](QHostAddress address, int tcpPort)
    {
        if(address == info.getAddress() && tcpPort == info.getTcpPort())
        {
            info.setStatus(NodeStatus::Incompatible);
            sendUpdateNodeInfo(info);

            obj->deleteLater();
        }
    });

    disconnectFromNodeWithoutNotification(info);
}
