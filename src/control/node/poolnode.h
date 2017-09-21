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

    PoolManagerInfo* getPoolManagerInfo() { return poolManager; }

    void enableTransmitter();
    void disableTransmitter();

signals:
    void sendLog(QString log);

private slots:
    void onNewConnection(QString hostName, QHostAddress address, int tcpPort);
    void onDisconnected(QString, QHostAddress, int);
    void onDataReceived(QByteArray*, QHostAddress, int);

private:
    void disconnectFromManager();

    PoolManagerInfo* poolManager;
};
