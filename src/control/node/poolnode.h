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

    PoolManagerInfo* getPoolManagerInfo() const { return poolManager; }

signals:
    void sendLog(QString log);

private slots:
    void onNewConnection(QHostAddress address, int tcpPort);
    void onDisconnected(QHostAddress, int);
    void onDataReceived(QByteArray* data, QHostAddress, int);

    void onGetVersion(QUuid uuid);

protected:
    void connectDispatcher();

private:
    void disconnectFromManager();

    void sendCommand(CommandType type, QByteArray* body = new QByteArray());
    void sendResponse(CommandType type, QUuid uuid, QByteArray* body = new QByteArray());

    PoolManagerInfo* poolManager;
};
