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
    PoolNode(Version programVersion, FileSystem& system, int selfPort = 0);
    ~PoolNode();

    void enable();
    void disable();

    PoolEntityInfo* getPoolManagerInfo() const { return poolManager; }

signals:
    void sendManagerAddress(QHostAddress address);

    void sendDisableManager();
    void sendEnableManager();
    void sendIncomingGrid(Grid* grid, int level, Statistics* statistics);

private slots:
    void onNewConnection(QHostAddress address, int tcpPort);
    void onDisconnected(QHostAddress address, int port);

    void onOK(QUuid uuid);
    void onError(QUuid uuid, QString what);
    void onIdentify(QUuid uuid, EntityType type);
    void onGetVersion(QUuid uuid);
    void onDisableManager(QUuid uuid);
    void onSendSessionDirectoryName(QUuid uuid, QString name);
    void onSendLibraryList(QUuid uuid, QList<Library*> libraries);
    void onSendArchitecture(QUuid uuid, Architecture* architecture);
    void onAssign(QUuid uuid);
    void onSendAssignedNode(QHostAddress address, int port);
    void onNoAssignedNode();
    void onSendScheme(QUuid uuid, Scheme* scheme, int initialLevel);
    void onSendGrid(QUuid uuid, Grid* grid, int level, Statistics* statistics);
    void onStop(QUuid uuid);

    void onSchemePart(Scheme* scheme, int level);
    void onNeedNodes(int level, int number);
    void sendSetEntityStatus(QHostAddress address, int port, EntityStatus status);

    void onClientError(QString error);

protected:
    void connectDispatcher();

private:
    void connectDistributor();
    void stop();

    static const EntityType entityType = EntityType::Node;

    PoolEntityInfo* poolManager;

    bool acceptNodeConnection;

    Client client;
    Distributor* distributor;

    QMap<int, int> neededNodes;
};
