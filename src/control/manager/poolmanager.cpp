#include "poolmanager.h"

PoolManager::PoolManager(Version programVersion, int selfPort) :
    PoolEntity(selfPort),
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

    sendClearNodesInfo();

    poolNodes.clear();

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
        sendUpdateNodeInfo(info);
        sendLog(tr("Success."));

        sendLog(tr("Requesting for program version..."));
        sendCommand(info, CommandType::GetVersion);
    }
    else
    {
        info.setStatus(NodeStatus::NotResponding);
        sendUpdateNodeInfo(info);
        sendLog(tr("Connection failed."));
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

void PoolManager::removeNode(PoolNodeInfo& info)
{
    if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
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

void PoolManager::sendCommand(PoolNodeInfo& info, CommandType type, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, body);
    transmitter->sendData(command->toByteArray(), info.getAddress(), info.getTcpPort());

    sentCommands.append(CommandHistoryEntry(info.getAddress(), info.getTcpPort(), command->getType(), command->getUuid()));

    delete command;
}

void PoolManager::sendResponse(PoolNodeInfo& info, CommandType type, QUuid uuid, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, uuid, body);
    transmitter->sendData(command->toByteArray(), info.getAddress(), info.getTcpPort());

    delete command;
}

PoolNodeInfo& PoolManager::getInfoByAddressAndPort(QHostAddress address, int port)
{
    for(PoolNodeInfo& info: poolNodes)
    {
        if(info.getAddress() == address && info.getTcpPort() == port)
            return info;
    }

    throw IllegalArgumentException(tr("Trying to access pool node at %1:%2 but node does not exist.")
                                   .arg(address.toString(), QString::number(port)));
}

void PoolManager::setSessionData(SessionData* data)
{
    delete this->data;
    this->data = data;
}

void PoolManager::onNewConnection(QHostAddress address, int tcpPort)
{
    transmitter->disconnectFromHost(address, tcpPort);

    sendLog(tr("Got an unexpected new connection from %1:%2. The connection was terminated.")
            .arg(address.toString(), QString::number(tcpPort)));
}

void PoolManager::onDisconnected(QHostAddress address, int tcpPort)
{
    try
    {
        PoolNodeInfo& info = getInfoByAddressAndPort(address, tcpPort);

        if(info.getProgramVersion() != programVersion)
            info.setStatus(NodeStatus::Incompatible);
        else
            info.setStatus(NodeStatus::NotResponding);
        sendUpdateNodeInfo(info);

        sendLog(tr("Lost connection with node at %1:%2.")
                .arg(address.toString(), QString::number(tcpPort)));
    }
    catch(Exception&)
    {
        sendLog(tr("Disconnected."));
    }
}

void PoolManager::onDataReceived(QByteArray* data, QHostAddress, int)
{
    Command* command = new Command(data);
    dispatcher.dispatchCommand(command);

    delete command;
}

void PoolManager::onSendVersion(QUuid uuid, QString version)
{
    CommandHistoryEntry& entry = getCommandHistoryEntry(uuid);
    PoolNodeInfo& info = getInfoByAddressAndPort(entry.getAddress(), entry.getPort());
    sentCommands.removeAll(entry);

    sendLog(tr("Received a program version from %1:%2.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    info.setProgramVersion(Version(version));
    sendUpdateNodeInfo(info);

    if(info.getProgramVersion() != programVersion)
    {
        disconnectFromNode(info);
        sendLog(tr("The node has an incompatible program version, disconnecting."));
    }
}

CommandHistoryEntry& PoolManager::getCommandHistoryEntry(QUuid uuid)
{
    for(CommandHistoryEntry& entry: sentCommands)
    {
        if(entry.getUuid() == uuid)
            return entry;
    }

    throw IllegalArgumentException(tr("Cannot find the command history entry with uuid %1.")
                                   .arg(uuid.toString()));
}
