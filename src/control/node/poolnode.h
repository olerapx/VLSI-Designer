#pragma once

#include "control/poolentity.h"
#include "poolmanagerinfo.h"

/**
 * @brief The PoolNode class
 * Represents a node performing a design process.
 */
class PoolNode : public PoolEntity
{
    Q_OBJECT

public:
    PoolNode(int selfPort = 0);
    ~PoolNode();

    PoolManagerInfo* getPoolManager() { return poolManager; }

    void enableTransmitter();
    void disableTransmitter();

private slots:
    void onNewConnection(QString hostName, QHostAddress address, int tcpPort);

private:
    void disconnectFromManager();

    PoolManagerInfo* poolManager;
};
