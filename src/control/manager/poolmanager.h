#pragma once

#include "control/poolentity.h"
#include "poolnodeinfo.h"

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

    void connectToUnconnectedNodes();

    void enableTransmitter();
    void disableTransmitter();

private slots:
    void onNewConnection(QString, QHostAddress address, int tcpPort);

private:
    bool tryConnect(PoolNodeInfo& info);

    QList<PoolNodeInfo> poolNodes;
};
