#pragma once

#include "control/poolentity.h"
#include "poolnodeinfo.h"
#include "exception/illegalargumentexception.h"

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

    void enable();
    void disable();

    void connectToUnconnectedNodes();
    void connectToNode(PoolNodeInfo& info);

    void removeNode(PoolNodeInfo& info);

    QList<PoolNodeInfo>& getPoolNodesInfo() { return poolNodes; }

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

    void disconnectFromNode(PoolNodeInfo& info);

    QList<PoolNodeInfo> poolNodes;
};
