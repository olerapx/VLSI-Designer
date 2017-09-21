#pragma once

#include "control/poolentity.h"
#include "poolnodeinfo.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The PoolManager class
 * Provides the management of the VLSI design process.
 */
class PoolManager : public PoolEntity
{
    Q_OBJECT

public:
    PoolManager(int selfPort = 0);

    QList<PoolNodeInfo>& getPoolNodesInfo() { return poolNodes; }

    void enableTransmitter();
    void disableTransmitter();

    void connectToUnconnectedNodes();
    void connectToNode(PoolNodeInfo& info);

    void removeNode(PoolNodeInfo& info);
    void disconnectFromNode(PoolNodeInfo& info);

signals:
    void sendUpdateNodeInfo(PoolNodeInfo& info);
    void sendRemoveNodeInfo(int index);
    void sendLog(QString log);

private slots:
    void onNewConnection(QString, QHostAddress address, int tcpPort);
    void onDisconnected(QString, QHostAddress, int tcpPort);
    void onDataReceived(QByteArray*, QHostAddress, int);

private:
    bool tryConnect(PoolNodeInfo& info);
    PoolNodeInfo& getInfoByAddressAndPort(QHostAddress address, int port);

    QList<PoolNodeInfo> poolNodes;
};
