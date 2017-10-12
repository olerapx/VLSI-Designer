#include "poolmanager.h"

PoolManager::PoolManager(Version programVersion, FileSystem& system, int selfPort) :
    PoolEntity(programVersion, system, selfPort)
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
    connect(&dispatcher, &CommandDispatcher::sendGetAvailableNode, this, &PoolManager::onGetAvailableNode, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSendGrid, this, &PoolManager::onSendGrid, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSetEntityStatus, this, &PoolManager::onSetEntityStatus, Qt::QueuedConnection);
}

void PoolManager::enable()
{
    if(transmitter != nullptr)
        return;

    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolManager::onNewConnection, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolEntity::onDataReceived, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDisconnected, this, &PoolManager::onDisconnected, Qt::QueuedConnection);

    sendLog(tr("Pool manager is enabled."));
}

void PoolManager::disable()
{
    if(transmitter == nullptr || isStarted())
        return;

    started = false;

    sendClearEntitiesInfo();

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
    if(info.getStatus() != EntityStatus::Unconnected && info.getStatus() != EntityStatus::NotResponding)
        return;

    sendLog(tr("Connecting to %1:%2...").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    info.setStatus(EntityStatus::Connecting);
    sendUpdateEntityInfo(info);

    if(tryConnect(info))
    {
        info.setStatus(EntityStatus::Ready);
        sendUpdateEntityInfo(info);
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
        info.setStatus(EntityStatus::NotResponding);
        sendUpdateEntityInfo(info);
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
    if(info.getStatus() == EntityStatus::Unconnected || info.getStatus() == EntityStatus::NotResponding)
    {
        int index = knownEntities.indexOf(info);
        knownEntities.removeAt(index);
        sendRemoveEntityInfo(index);

        return;
    }

    QObject* obj = new QObject(this);
    QObject::connect(this, &PoolManager::sendDisconnected, obj, [this, obj, &info](QHostAddress address, int tcpPort)
    {
        if(address == info.getAddress() && tcpPort == info.getTcpPort())
        {
            int index = knownEntities.indexOf(info);

            knownEntities.removeAt(index);
            sendRemoveEntityInfo(index);

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
            info.setStatus(EntityStatus::Unconnected);
            sendUpdateEntityInfo(info);

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
    if(isStarted())
        return;

    started = true;

    setStatusOfAllConnectedNodes(EntityStatus::Initialization);
    disableManagers();
    createSession();
}

void PoolManager::stop()
{
    if(!isStarted())
        return;

    for(PoolEntityInfo& info: knownEntities)
    {
        sendUntrackedRequest(info.getAddress(), info.getTcpPort(), CommandType::Stop);

        info.setStatus(EntityStatus::Ready);
        sendUpdateEntityInfo(info);
    }

    started = false;
}

void PoolManager::setStatusOfAllConnectedNodes(EntityStatus status)
{
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() != EntityStatus::Unconnected && info.getStatus() != EntityStatus::NotResponding)
        {
            info.setStatus(status);
            sendUpdateEntityInfo(info);
        }
    }
}

void PoolManager::disableManagers()
{
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() != EntityStatus::Unconnected && info.getStatus() != EntityStatus::NotResponding)
            sendUntrackedRequest(info.getAddress(), info.getTcpPort(), CommandType::DisableManager);
    }
}

void PoolManager::createSession()
{
    sendLog(tr("Sending session name to all nodes."));
    fileSystem.setSessionName(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));

    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() != EntityStatus::Unconnected && info.getStatus() != EntityStatus::NotResponding)
             startNodeInitialization(info);
    }
}

void PoolManager::startNodeInitialization(PoolEntityInfo& info)
{
    QByteArray* body = new QByteArray();
    body->append(fileSystem.getSessionName() + QString(" %1").arg(QString::number(info.getTcpPort())));
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

    info.setStatus(EntityStatus::Ready);
    sendUpdateEntityInfo(info);

    bool allInitialized = true;
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getStatus() == EntityStatus::Initialization)
        {
            allInitialized = false;
            break;
        }
    }

    if(!allInitialized)
        return;

    sendLog(tr("Initialization finished."), LogType::Success);

    BinarySerializer serializer;

    QByteArray schemeArray = serializer.serialize(data->getScheme());

    QByteArray* body = new QByteArray(schemeArray);
    body->prepend(sizeof(qint32), ' ');

    QDataStream stream(body, QIODevice::WriteOnly);
    stream.device()->seek(0);
    stream << (qint32)0;

    sendLog(tr("Starting the design process."));
    sendLog(tr("Sending scheme to %1:%2.").arg(knownEntities[0].getAddress().toString(), QString::number(knownEntities[0].getTcpPort())));
    knownEntities[0].setStatus(EntityStatus::Working);
    sendUpdateEntityInfo(knownEntities[0]);

    sendRequest(knownEntities[0].getAddress(), knownEntities[0].getTcpPort(), CommandType::SendScheme, body);
}

void PoolManager::onNewConnection(QHostAddress address, int tcpPort)
{
    transmitter->disconnectFromHost(address, tcpPort);
    sendLog(tr("Got an unexpected new connection from %1:%2. The connection was terminated.").arg(address.toString(), QString::number(tcpPort)));
}

void PoolManager::onDisconnected(QHostAddress address, int tcpPort)
{
    PoolEntityInfo& info = getInfoByAddressAndPort(address, tcpPort);

    info.setStatus(EntityStatus::NotResponding);
    sendUpdateEntityInfo(info);

    sendLog(tr("Lost connection with node at %1:%2.").arg(address.toString(), QString::number(tcpPort)), LogType::Warning);

    sendDisconnected(address, tcpPort);
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
    case CommandType::Assign:
        sendAssignedNode(info, uuid);
        break;
    default:
        return;
    }
}

void PoolManager::onError(QUuid uuid, QString what)
{
    CommandType previousCommandType = getCommandHistoryEntry(outcomingRequests, uuid).getType();
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);

    switch(previousCommandType)
    {
    case CommandType::Assign:
        sendCannotAssignNode(uuid);
        break;
    default:
        return;
    }

    info.setStatus(EntityStatus::Error);
    sendUpdateEntityInfo(info);

    sendLog(what, LogType::Warning);
}

void PoolManager::onSendVersion(QUuid uuid, Version version)
{
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);

    sendLog(tr("Received a program version from %1:%2.").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    info.setProgramVersion(version);
    sendUpdateEntityInfo(info);

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
            info.setStatus(EntityStatus::Incompatible);
            sendUpdateEntityInfo(info);

            obj->deleteLater();
        }
    });

    disconnectFromNodeWithoutNotification(info);
}

void PoolManager::onGetAvailableNode(QUuid uuid)
{
    CommandHistoryEntry& entry = getCommandHistoryEntry(incomingRequests, uuid);
    PoolEntityInfo& info = getInfoByAddressAndPort(entry.getAddress(), entry.getPort());

    for(PoolEntityInfo& i: knownEntities)
    {
        if(i.getStatus() == EntityStatus::Ready && !(i == info))
        {
            i.setStatus(EntityStatus::Assigned);
            sendUpdateEntityInfo(i);

            sendResponse(i.getAddress(), i.getTcpPort(), CommandType::Assign, uuid);

            outcomingRequests.append(CommandHistoryEntry(i.getAddress(), i.getTcpPort(), CommandType::Assign, uuid));
            return;
        }
    }

    QByteArray* body = new QByteArray(tr("An available node was not found.").toUtf8());

    sendLog(tr("An available node was not found."), LogType::Warning);
    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::Error, uuid, body);
}

void PoolManager::sendAssignedNode(PoolEntityInfo& info, QUuid uuid)
{
    PoolEntityInfo& senderInfo = removeRequestFromList(incomingRequests, uuid);

    sendLog(tr("An available node was found at %1:%2, sending to %3:%4.").arg(info.getAddress().toString(), QString::number(info.getTcpPort()),
                                                                           senderInfo.getAddress().toString(), QString::number(senderInfo.getTcpPort())), LogType::Success);

    QByteArray* body = new QByteArray();
    QDataStream stream(body, QIODevice::WriteOnly);
    stream << info.getAddress().toString() << info.getTcpPort();

    sendResponse(senderInfo.getAddress(), senderInfo.getTcpPort(), CommandType::SendAssignedNode, uuid, body);
}

void PoolManager::sendCannotAssignNode(QUuid uuid)
{
    PoolEntityInfo& senderInfo = removeRequestFromList(incomingRequests, uuid);

    QByteArray* body = new QByteArray(tr("Failed to connect to available node.").toUtf8());
    sendLog(tr("Failed to connect to available node."), LogType::Warning);

    sendResponse(senderInfo.getAddress(), senderInfo.getTcpPort(), CommandType::Error, uuid, body);
}

void PoolManager::onSendGrid(QUuid uuid, Grid* grid, int, Statistics* statistics)
{
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);
    sendLog(tr("Node %1:%2 has finished the design process.").arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Success);

    info.setStatus(EntityStatus::Ready);
    sendUpdateEntityInfo(info);

    stop();
    sendFinish(statistics, grid);
}

void PoolManager::onSetEntityStatus(QUuid uuid, QHostAddress address, int port, EntityStatus status)
{
    removeRequestFromList(incomingRequests, uuid);

    for(PoolEntityInfo& info: knownEntities)
    {
        if(info == PoolEntityInfo("", address, port))
        {
            info.setStatus(status);
            sendUpdateEntityInfo(info);
            return;
        }
    }
}
