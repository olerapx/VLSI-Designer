#include "poolnode.h"

PoolNode::PoolNode(QString sessionPath, Version programVersion, int selfPort) :
    PoolEntity(selfPort),
    sessionPath(sessionPath),
    programVersion(programVersion),
    poolManager(nullptr),
    acceptNodeConnection(false)
{
    connectDispatcher();

    connect(&client, &Client::sendLog, this, &PoolEntity::sendLog);
    connect(&client, &Client::sendError, this, &PoolEntity::sendError);
}

PoolNode::~PoolNode()
{
    disable();
}

void PoolNode::connectDispatcher()
{
    connect(&dispatcher, &CommandDispatcher::sendOK, this, &PoolNode::onOK, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendError, this, &PoolNode::onError, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendIdentify, this, &PoolNode::onIdentify, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendGetVersion, this, &PoolNode::onGetVersion, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendDisableManager, this, &PoolNode::onDisableManager, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSendSessionDirectoryName, this, &PoolNode::onSendSessionDirectoryName, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSendLibraryList, this, &PoolNode::onSendLibraryList, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSendArchitecture, this, &PoolNode::onSendArchitecture, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendAssign, this, &PoolNode::onAssign, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSendAssignedNode, this, &PoolNode::onSendAssignedNode, Qt::QueuedConnection);
}

void PoolNode::enable()
{
    if(transmitter != nullptr)
        return;

    acceptNodeConnection = false;

    PoolEntity::enableTransmitter();
    connect(transmitter, &NetworkTransmitter::sendNewConnection, this, &PoolNode::onNewConnection, Qt::QueuedConnection);
    connect(transmitter, &NetworkTransmitter::sendDataReceived, this, &PoolEntity::onDataReceived, Qt::QueuedConnection);
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

    client.sendStop();

    poolManager = nullptr;
    PoolEntity::disableTransmitter();

    sendLog(tr("Pool node is disabled."));
}

void PoolNode::onNewConnection(QHostAddress address, int tcpPort)
{
    sendLog(tr("Got a new connection from %1:%2, waiting for identification.").arg(address.toString(), QString::number(tcpPort)));
    knownEntities.append(PoolEntityInfo("", address, tcpPort));
}

void PoolNode::onDisconnected(QHostAddress address, int port)
{
    sendLog(tr("Lost connection with %1:%2.").arg(address.toString(), QString::number(port)), LogType::Warning);

    bool managerDisconnected = false;

    if(address == knownEntities[0].getAddress() && port == knownEntities[0].getTcpPort() && poolManager != nullptr)
    {
        poolManager = nullptr;
        managerDisconnected = true;
    }

    int index = knownEntities.indexOf(PoolEntityInfo("", address, port));
    knownEntities.removeAt(index);
    sendRemoveNodeInfo(index);

    if(managerDisconnected)
    {
        sendLog(tr("Connection with manager was lost."), LogType::Warning);

        for(PoolEntityInfo& info: knownEntities)
            transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
    }
}

void PoolNode::onOK(QUuid uuid)
{
    Q_UNUSED(uuid)
    // TODO
}

void PoolNode::onError(QUuid uuid, QString what)
{
    CommandType previousCommandType = getCommandHistoryEntry(outcomingRequests, uuid).getType();
    removeRequestFromList(outcomingRequests, uuid);

    switch(previousCommandType)
    {
    case CommandType::GetAvailableNode:
        break;
    default:
        break;
    }

    sendLog(what, LogType::Warning);
}

void PoolNode::onIdentify(QUuid uuid, EntityType type)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    QString typeString = ((type == EntityType::Manager) ? tr("manager") : tr("node"));

    sendLog(tr("Connection from %1:%2 identified as %3.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort()), typeString));

    if(type == EntityType::Manager)
    {
        if(poolManager != nullptr)
        {
            transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
            sendLog(tr("Connection with %1:%2 was aborted: already have a connected pool manager.")
                    .arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Warning);

            knownEntities.removeAll(info);

            return;
        }

        poolManager = &(knownEntities[0]);

        knownEntities[0].setStatus(NodeStatus::Manager);
        sendUpdateNodeInfo(knownEntities[0]);

        return;
    }

    if(acceptNodeConnection)
    {
        acceptNodeConnection = false;

        info.setStatus(NodeStatus::Node);
        sendUpdateNodeInfo(info);
        return;
    }

    transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
    sendLog(tr("Connection with %1:%2 was aborted: have not received the assigning command.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort())), LogType::Warning);

    knownEntities.removeAll(info);
}

void PoolNode::onGetVersion(QUuid uuid)
{
    try
    {
        PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

        QByteArray* body = new QByteArray();
        QDataStream stream(body, QIODevice::WriteOnly);

        stream << programVersion.toString();

        sendLog(tr("Sending response on program version request."));
        sendResponse(info.getAddress(), info.getTcpPort(), CommandType::SendVersion, uuid, body);
    }
    catch(Exception&)
    {

    }
}

void PoolNode::onDisableManager(QUuid uuid)
{
    removeRequestFromList(incomingRequests, uuid);

    sendLog(tr("Received a request to disable the manager."));
    sendDisableManager();
}

void PoolNode::onSendSessionDirectoryName(QUuid uuid, QString name)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);
    currentSessionName = name;

    QDir dir(getCurrentSessionPath());
    dir.mkpath(".");

    sendLog(tr("Received current session name from manager."), LogType::Success);
    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::OK, uuid);
}

void PoolNode::onSendLibraryList(QUuid uuid, QList<Library*> libraries)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    QDir dir(getCurrentSessionPath() + "/libraries");
    dir.mkpath(".");

    client.setLibraries(libraries);

    JsonSerializer serializer;

    for(Library* l: libraries)
    {
        QFile f(QString("%1/%2-%3.json").arg(dir.absolutePath(), l->getId(), l->getVersion().toString()));
        f.open(QIODevice::WriteOnly);

        f.write(serializer.serialize(l));
        f.close();
    }

    sendLog(tr("Received libraries list from manager."), LogType::Success);
    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::OK, uuid);
}

void PoolNode::onSendArchitecture(QUuid uuid, Architecture* architecture)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    client.setArchitecture(architecture);

    QDir dir(getCurrentSessionPath() + "/architecture");
    dir.mkpath(".");

    QFile f(QString("%1/%2.json").arg(dir.absolutePath(), "architecture"));
    f.open(QIODevice::WriteOnly);

    JsonSerializer serializer;
    f.write(serializer.serialize(architecture));
    f.close();

    sendLog(tr("Received architecture from manager."), LogType::Success);
    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::OK, uuid);
}

void PoolNode::onAssign(QUuid uuid)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    if(acceptNodeConnection)
    {
        QByteArray* body = new QByteArray();
        QString error = tr("Assigning request denied, already assigned to a node.");
        body->append(error);

        sendResponse(info.getAddress(), info.getTcpPort(), CommandType::Error, uuid, body);
        sendLog(error, LogType::Error);

        return;
    }

    acceptNodeConnection = true;

    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::OK, uuid);
    sendLog(tr("Assigning request approved."), LogType::Success);
}

QString PoolNode::getCurrentSessionPath()
{
    return(sessionPath + "/" + currentSessionName);
}

void PoolNode::onSendAssignedNode(QHostAddress address, int port)
{
    sendLog(tr("Received the available node."), LogType::Success);

    try
    {
        transmitter->connectToHost(address, port);
        knownEntities.append(PoolEntityInfo("", address, port));

        knownEntities.last().setStatus(NodeStatus::Node);
        sendUpdateNodeInfo(knownEntities.last());

        sendLog(tr("Connected to the available node."), LogType::Success);
    }
    catch(Exception&)
    {
        sendLog(tr("Cannot connect to the available node."), LogType::Warning);
    }
}
