#pragma once

#include "control/poolentity.h"
#include "poolmanagerinfo.h"

/**
 * @brief The PoolNode class
 * Represents a node performing a design process.
 * In order to get node working, you should call enable().
 */
class PoolNode : public PoolEntity
{
    Q_OBJECT

public:
    /**
     * @brief PoolNode
     * Constructs the object.
     * @param selfPort - the port the transmitter will be set to.
     */
    PoolNode(int selfPort = 0);
    ~PoolNode();

    void enable();
    void disable();

    PoolManagerInfo* getPoolManagerInfo() { return poolManager; }

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
