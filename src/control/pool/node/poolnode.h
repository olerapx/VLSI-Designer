#pragma once

#include <QDir>

#include "control/pool/poolentity.h"
#include "datamodels/version/version.h"
#include "util/serializer/jsonserializer.h"
#include "control/distribution/defaultdistributor.h"

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
    PoolNode(QString sessionPath, Version programVersion, int selfPort = 0);
    ~PoolNode();

    void enable();
    void disable();

    PoolEntityInfo* getPoolManagerInfo() const { return poolManager; }

signals:
    void sendDisableManager();

private slots:
    void onNewConnection(QHostAddress address, int tcpPort);
    void onDisconnected(QHostAddress address, int port);
    void onDataReceived(QByteArray* data, QHostAddress address, int port);

    void onOK(QUuid uuid);
    void onError(QUuid uuid, QString what);
    void onIdentify(QUuid uuid, EntityType type);
    void onGetVersion(QUuid uuid);
    void onDisableManager(QUuid uuid);
    void onSendSessionDirectoryName(QUuid uuid, QString name);
    void onSendLibraryList(QUuid uuid, QList<Library*> libraries);
    void onSendArchitecture(QUuid uuid, Architecture* architecture);
    void onAssign(QUuid uuid);

protected:
    void connectDispatcher();

private:
    QString getCurrentSessionPath();

    static const EntityType entityType = EntityType::Node;

    QString sessionPath;
    QString currentSessionName;

    Version programVersion;
    PoolEntityInfo* poolManager;

    bool acceptNodeConnection;

    Client client;
};
