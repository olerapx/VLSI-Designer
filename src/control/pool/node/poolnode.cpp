#include "poolnode.h"

PoolNode::PoolNode(Version programVersion, FileSystem& system, int selfPort) :
    PoolEntity(programVersion, system, selfPort),
    poolManager(nullptr),
    acceptNodeConnection(false),
    distributor(nullptr)
{
    connectDispatcher();

    connect(&client, &Client::sendLog, this, &PoolEntity::sendLog);
    connect(&client, &Client::sendError, this, &PoolEntity::sendError);
}

PoolNode::~PoolNode()
{
    disable();
    delete distributor;
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
    connect(&dispatcher, &CommandDispatcher::sendSendScheme, this, &PoolNode::onSendScheme, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendSendGrid, this, &PoolNode::onSendGrid, Qt::QueuedConnection);
    connect(&dispatcher, &CommandDispatcher::sendStop, this, &PoolNode::onStop, Qt::QueuedConnection);
}

void PoolNode::connectDistributor()
{
    if(distributor == nullptr)
        return;

    connect(distributor, &Distributor::sendSchemePart, this, &PoolNode::onSchemePart);
    connect(distributor, &Distributor::sendNeedNodes, this, &PoolNode::onNeedNodes);
    connect(this, &PoolNode::sendIncomingGrid, distributor, &Distributor::onIncomingGrid, Qt::QueuedConnection);
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

    client.stop();

    poolManager = nullptr;
    PoolEntity::disableTransmitter();

    sendLog(tr("Pool node is disabled."));
}

void PoolNode::onNewConnection(QHostAddress address, int tcpPort)
{
    sendLog(tr("Got a new connection from %1:%2, waiting for identification.").arg(address.toString(), QString::number(tcpPort)));
    knownEntities.append(PoolEntityInfo("", address, tcpPort));
    sendAddEntityInfo();
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
    sendRemoveEntityInfo(index);

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
        onNoAssignedNode();
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

        knownEntities[0].setStatus(EntityStatus::Manager);
        sendUpdateEntityInfo(knownEntities[0]);

        return;
    }

    if(acceptNodeConnection)
    {
        acceptNodeConnection = false;

        info.setStatus(EntityStatus::Node);
        sendUpdateEntityInfo(info);
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
    fileSystem.setSessionName(name);

    QDir dir(fileSystem.getSessionPath());
    dir.mkpath(".");

    sendLog(tr("Received current session name from manager."), LogType::Success);
    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::OK, uuid);
}

void PoolNode::onSendLibraryList(QUuid uuid, QList<Library*> libraries)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    QDir dir(fileSystem.getLibrariesPath());
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

    delete distributor;
    distributor = nullptr;

    if(architecture->getDistributionType() == DistributionType::Default)
        distributor = new DefaultDistributor(client, fileSystem);

    connectDistributor();

    QDir dir(fileSystem.getArchitecturePath());
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
        QString error = tr("Assigning request was denied by the node: already assigned.");
        body->append(error);

        sendResponse(info.getAddress(), info.getTcpPort(), CommandType::Error, uuid, body);
        sendLog(tr("Assigning request denied: already assigned to a node."), LogType::Warning);

        return;
    }

    acceptNodeConnection = true;

    sendResponse(info.getAddress(), info.getTcpPort(), CommandType::OK, uuid);
    sendLog(tr("Assigning request approved."), LogType::Success);
}

void PoolNode::onNeedNodes(int level, int number)
{
    sendLog(tr("Requesting %1 node(s) from manager.").arg(QString::number(number)));
    neededNodes.insert(level, number);

    for(int i=0; i<number; i++)
    {
        sendRequest(poolManager->getAddress(), poolManager->getTcpPort(), CommandType::GetAvailableNode);
    }
}

void PoolNode::onSendAssignedNode(QHostAddress address, int port)
{
    sendLog(tr("Received the available node: %1:%2.").arg(address.toString(), QString::number(port)), LogType::Success);

    neededNodes[neededNodes.firstKey()] --;

    try
    {
        transmitter->connectToHost(address, port);
        knownEntities.append(PoolEntityInfo("", address, port));

        knownEntities.last().setStatus(EntityStatus::Node);
        sendAddEntityInfo();

        QByteArray* body = new QByteArray();
        QDataStream stream(body, QIODevice::WriteOnly);
        stream << (qint32) entityType;

        sendUntrackedRequest(address, port, CommandType::Identify, body);
        sendLog(tr("Connected to the available node."), LogType::Success);
    }
    catch(Exception&)
    {
        sendLog(tr("Cannot connect to the available node."), LogType::Warning);
    }

    if(neededNodes.first() == 0)
    {
        int level = neededNodes.firstKey();
        neededNodes.remove(level);
        distributor->onReceivedNodes(level);
    }
}

void PoolNode::onNoAssignedNode()
{
    neededNodes[neededNodes.firstKey()] --;

    if(neededNodes.first() == 0)
    {
        int level = neededNodes.firstKey();
        neededNodes.remove(level);
        distributor->onReceivedNodes(level);
    }
}

void PoolNode::onSchemePart(Scheme* scheme, int level)
{
    sendLog(tr("Sending scheme part to any available node."));

    for(int i=1; i<knownEntities.size(); i++)
    {
        if(knownEntities[i].getStatus() == EntityStatus::Node)
        {
            knownEntities[i].setStatus(EntityStatus::Working);
            sendUpdateEntityInfo(knownEntities[i]);

            sendLog(tr("Sending scheme to %1:%2.").arg(knownEntities[i].getAddress().toString(), QString::number(knownEntities[i].getTcpPort())));

            BinarySerializer serializer;

            QByteArray schemeArray = serializer.serialize(scheme);
            delete scheme;

            QByteArray* body = new QByteArray(schemeArray);
            body->prepend(sizeof(qint32), ' ');

            QDataStream stream(body, QIODevice::WriteOnly);
            stream.device()->seek(0);
            stream << (qint32)level;

            knownEntities[i].setStatus(EntityStatus::Working);
            sendUpdateEntityInfo(knownEntities[i]);

            sendRequest(knownEntities[i].getAddress(), knownEntities[i].getTcpPort(), CommandType::SendScheme, body);
            sendSetEntityStatus(knownEntities[i].getAddress(), knownEntities[i].getTcpPort(), EntityStatus::Working);

            return;
        }
    }

    sendLog(tr("Cannot find available node, handling the scheme part on one's own."), LogType::Warning);
    distributor->start(scheme, level);
}

void PoolNode::onSendScheme(QUuid uuid, Scheme* scheme, int initialLevel)
{
    PoolEntityInfo& info = removeRequestFromList(incomingRequests, uuid);

    sendLog(tr("Received scheme from %1:%2 on distribution level %3. Start handling.")
            .arg(info.getAddress().toString(), QString::number(info.getTcpPort()), QString::number(initialLevel)));

    QObject* obj = new QObject(this);

    QObject::connect(&dispatcher, &CommandDispatcher::sendStop, obj, [this, obj](QUuid)
    {
        obj->deleteLater();
    });

    QObject::connect(distributor, &Distributor::sendResult, obj, [this, obj, info, uuid, scheme, initialLevel] (Grid* grid, int level)
    {
        if(level != initialLevel)
        {
            sendIncomingGrid(grid, level);
            return;
        }

        BinarySerializer serializer;

        QByteArray gridArray = serializer.serialize(grid);
        delete grid;

        QByteArray* body = new QByteArray(gridArray);
        body->prepend(sizeof(qint32), ' ');

        QDataStream stream(body, QIODevice::WriteOnly);
        stream.device()->seek(0);
        stream << (qint32)level;

        sendLog(tr("Finished handling scheme, sending back to %1:%2.").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));
        sendResponse(info.getAddress(), info.getTcpPort(), CommandType::SendGrid, uuid, body);

        obj->deleteLater();
    });

    distributor->start(scheme, initialLevel);
}

void PoolNode::onSendGrid(QUuid uuid, Grid* grid, int level)
{
    PoolEntityInfo& info = removeRequestFromList(outcomingRequests, uuid);
    sendLog(tr("Received grid from %1:%2.").arg(info.getAddress().toString(), QString::number(info.getTcpPort())));

    transmitter->disconnectFromHost(info.getAddress(), info.getTcpPort());
    sendSetEntityStatus(info.getAddress(), info.getTcpPort(), EntityStatus::Ready);
    sendRemoveEntityInfo(knownEntities.indexOf(info));

    sendIncomingGrid(grid, level);
}

void PoolNode::onStop(QUuid uuid)
{
    removeRequestFromList(incomingRequests, uuid);
    sendLog(tr("Received a stop request."));

    sendEnableManager();
    distributor->stop();

    for(int i=1; i<knownEntities.size(); i++)
        transmitter->disconnectFromHost(knownEntities[i].getAddress(), knownEntities[i].getTcpPort());
}

void PoolNode::sendSetEntityStatus(QHostAddress address, int port, EntityStatus status)
{
    QByteArray* body = new QByteArray();
    QDataStream stream(body, QIODevice::WriteOnly);
    stream << address.toString() << port << (qint32) status;

    sendUntrackedRequest(poolManager->getAddress(), poolManager->getTcpPort(), CommandType::SetEntityStatus, body);
}
