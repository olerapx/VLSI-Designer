#pragma once

#include <QNetworkInterface>
#include "network/networkscanner.h"

#include "poolnodeinfo.h"

/**
 * @brief The PoolManager class
 * Provides the management of the VLSI design process.
 */
class PoolManager
{
public:
    PoolManager(int port = 0);

    QList<PoolNodeInfo>& getPoolNodes() { return poolNodes; }

    int getPort() { return port; }
    void setPort(int port);

private:
    QList<PoolNodeInfo> poolNodes;

    int port;
};
