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
    connect(&dispatcher, &CommandDispatcher::sendOK, this, &PoolManager::onOK, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendError, this, &PoolManager::onError, Qt::QueuedConnection);
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
    if(transmitter == nullptr || isStarted())
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
    for(PoolEntityInfo& info: knownEntities)
        connectToNode(info);
}

void PoolManager::connectToNode(PoolEntityInfo &info)
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
        sendLog(tr("Success."), LogType::Success);

        QByteArray* body = new QByteArray();
        QDataStream stream(body, QIODevice::WriteOnly);
        stream << (qint32) entityType;

        sendUntrackedRequest(info.getAddress(), info.getTcpPort(), CommandType::Identify, body);

        sendLog(tr("Requesting for program version..."));
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
        int index = knownEntities.indexOf(info);
        knownEntities.removeAt(index);
        sendRemoveNodeInfo(index);

        return;
    }

    QObject* obj = new QObject(this);
    QObject::connect(this, &PoolManager::sendDisconnected, obj, [this, obj, &info](QHostAddress address, int tcpPort)
    {
        if(address == info.getAddress() && tcpPort == info.getTcpPort())
        {
            int index = knownEntities.indexOf(info);

            knownEntities.removeAt(index);
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
    sendLog(tr("Disconnecting from %1:%2...").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));
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
    disableManagers();
    createSession();
}

void PoolManager::setStatusOfAllConnectedNodes(NodeStatus status)
{
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
        {
            info.setStatus(status);
            sendUpdateNodeInfo(info);
        }
    }
}

void PoolManager::disableManagers()
{
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
            sendUntrackedRequest(info.getAddress(), info.getTcpPort(), CommandType::DisableManager);
    }
}

void PoolManager::createSession()
{
    sendLog(tr("Sending session name to all nodes."));
    currentSessionName = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");

    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() != NodeStatus::Unconnected && info.getStatus() != NodeStatus::NotResponding)
             startNodeInitialization(info);
    }
}

void PoolManager::startNodeInitialization(PoolEntityInfo& info)
{
    QByteArray* body = new QByteArray();
    body->append(currentSessionName + QString(" %1").arg(QString::number(info.getTcpPort())));
    sendRequest(info.getAddress(), info.getTcpPort(), CommandType::SendSessionDirectoryName, body);
}

void PoolManager::sendLibraryListToNode(PoolEntityInfo& info)
{
    sendLog(tr("Current session name was accepted by node %1:%2. Sending library list.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    QByteArray* body = new QByteArray();
    QDataStream stream(body, QIODevice::WriteOnly);

    stream << (qint32) data->getLibraries().size();
    BinarySerializer serializer;

    for(Library* library: data->getLibraries())
    {
        QByteArray array = serializer.serialize(library);
        stream << static_cast<quint32>(array.size());
        stream.writeRawData(array.constData(), array.size());
    }

    sendRequest(info.getAddress(), info.getTcpPort(), CommandType::SendLibraryList, body);
}

void PoolManager::sendArchitectureToNode(PoolEntityInfo& info)
{
    sendLog(tr("Library list was accepted by node %1:%2. Sending architecture.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    BinarySerializer serializer;
    QByteArray architecture = serializer.serialize(data->getArchitecture());

    QByteArray* body = new QByteArray(architecture);

    sendRequest(info.getAddress(), info.getTcpPort(), CommandType::SendArchitecture, body);
}

void PoolManager::markNodeInitialized(PoolEntityInfo& info)
{
    sendLog(tr("Node %1:%2 is initialized.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Success);

    info.setStatus(NodeStatus::Ready);
    sendUpdateNodeInfo(info);

    bool allInitialized = true;
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() == NodeStatus::Initialization)
        {
            allInitialized = false;
            break;
        }
    }

    if(!allInitialized)
        return;

    sendLog(tr("Initialization finished."), LogType::Success);
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

void PoolManager::onOK(QUuid uuid)
{
    CommandType previousCommandType = getCommandHistoryEntry(outcomingRequests, uuid).getType();
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);

    switch(previousCommandType)
    {
    case CommandType::SendSessionDirectoryName:
        sendLibraryListToNode(info);
        break;
    case CommandType::SendLibraryList:
        sendArchitectureToNode(info);
        break;
    case CommandType::SendArchitecture:
        markNodeInitialized(info);
        break;
    default:
        return;
    }
}

void PoolManager::onError(QUuid uuid, QString what)
{
    sendError(what);

    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);

    info.setStatus(NodeStatus::Error);
    sendUpdateNodeInfo(info);
}

void PoolManager::onSendVersion(QUuid uuid, Version version)
{
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);

    sendLog(tr("Received a program version from %1:%2.").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    info.setProgramVersion(version);
    sendUpdateNodeInfo(info);

    if(info.getProgramVersion() == programVersion)
    {
        sendLog(tr("Version is compatible."), LogType::Success);

        if(isStarted())
            startNodeInitialization(info);

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
