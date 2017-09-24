#pragma once

#include "control/poolentity.h"
#include "poolnodeinfo.h"
#include "exception/illegalargumentexception.h"
#include "sessiondata.h"
#include "commandhistoryentry.h"

/**
 * @brief The PoolManager class
 * Provides the management of the VLSI design process.
 * In order to get manager working, you should call enable().
 */
class PoolManager : public PoolEntity
{
    Q_OBJECT

public:
    /**
     * @brief PoolManager
     * Constructs the object.
     * @param selfPort - the port the transmitter will be set to.
     */
    PoolManager(int selfPort = 0);
    ~PoolManager();

    void enable();
    void disable();

    void connectToUnconnectedNodes();
    void connectToNode(PoolNodeInfo& info);

    void removeNode(PoolNodeInfo& info);

    QList<PoolNodeInfo>& getPoolNodesInfo() { return poolNodes; }

    SessionData* getSessionData() { return data; }
    void setSessionData(SessionData* data);

signals:
    void sendUpdateNodeInfo(PoolNodeInfo& info);
    void sendRemoveNodeInfo(int index);
    void sendLog(QString log);

private slots:
    void onNewConnection(QHostAddress address, int tcpPort);
    void onDisconnected(QHostAddress, int tcpPort);
    void onDataReceived(QByteArray* data, QHostAddress, int);

    void onSendVersion(QUuid uuid, double version);
    CommandHistoryEntry& getCommandHistoryEntry(QUuid uuid);

protected:
    void connectDispatcher();

private:
    bool tryConnect(PoolNodeInfo& info);
    PoolNodeInfo& getInfoByAddressAndPort(QHostAddress address, int port);

    void disconnectFromNode(PoolNodeInfo& info);

    void sendCommand(PoolNodeInfo& info, CommandType type, QByteArray* body = new QByteArray());
    void sendResponse(PoolNodeInfo& info, CommandType type, QUuid uuid, QByteArray* body = new QByteArray());

    QList<PoolNodeInfo> poolNodes;
    SessionData* data;

    QList<CommandHistoryEntry> sentCommands;
};
