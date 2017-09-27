#pragma once

#include "control/poolentity.h"
#include "datamodels/version/version.h"

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
    PoolNode(Version programVersion, int selfPort = 0);
    ~PoolNode();

    void enable();
    void disable();

    PoolEntityInfo* getPoolManagerInfo() const { return poolManager; }

signals:
    void sendLog(QString log, LogType type = LogType::Common);
    void sendError(QString error);

    void sendConnectionIdentified();

private slots:
    void onNewConnection(QHostAddress address, int tcpPort);
    void onDisconnected(QHostAddress address, int port);
    void onDataReceived(QByteArray* data, QHostAddress address, int port);

    void onIdentify(QUuid uuid, EntityType type);
    void onGetVersion(QUuid uuid);

protected:
    void connectDispatcher();

private:
    static const EntityType entityType = EntityType::Node;

    Version programVersion;
    PoolEntityInfo* poolManager;
};
