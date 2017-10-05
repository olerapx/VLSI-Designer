#pragma once

#include "network/networktransmitter.h"
#include "control/command/commanddispatcher.h"
#include "poolentityinfo.h"
#include "entitytype.h"
#include "logtype.h"
#include "manager/commandhistoryentry.h"
#include "control/filesystem/filesystem.h"

/**
 * @brief The PoolEntity class
 * Base class for pool objects such as manager and node.
 */
class PoolEntity : public QObject
{
    Q_OBJECT

public:
    PoolEntity(Version programVersion, FileSystem& system, int selfPort = 0);
    virtual ~PoolEntity();

    int getSelfPort() const { return selfPort; }
    void setSelfPort(int selfPort);

    void enableTransmitter();
    void disableTransmitter();

    bool isTransmitterEnabled() { return (transmitter != nullptr); }

    QList<PoolEntityInfo>& getKnownEntities() { return knownEntities; }

signals:
    void sendLog(QString log, LogType type = LogType::Common);
    void sendError(QString error);

    void sendClearEntitiesInfo();
    void sendUpdateEntityInfo(PoolEntityInfo& info);
    void sendRemoveEntityInfo(int index);
    void sendAddEntityInfo();

public slots:
    void onDataReceived(QByteArray* data, QHostAddress address, int port);

protected:
    virtual void connectDispatcher() = 0;
    CommandHistoryEntry& getCommandHistoryEntry(QList<CommandHistoryEntry>& list, QUuid uuid);

    void sendRequest(QHostAddress address, int port, CommandType type, QByteArray* body = new QByteArray());
    void sendUntrackedRequest(QHostAddress address, int port, CommandType type, QByteArray* body = new QByteArray());

    void sendResponse(QHostAddress address, int port, CommandType type, QUuid uuid, QByteArray* body = new QByteArray());

    PoolEntityInfo& removeRequestFromList(QList<CommandHistoryEntry>& list, QUuid uuid);
    PoolEntityInfo& getInfoByAddressAndPort(QHostAddress address, int port);

    NetworkTransmitter* transmitter;
    CommandDispatcher dispatcher;

    Version programVersion;
    FileSystem& fileSystem;
    int selfPort;

    QList<PoolEntityInfo> knownEntities;

    QList<CommandHistoryEntry> outcomingRequests;
    QList<CommandHistoryEntry> incomingRequests;    
};
