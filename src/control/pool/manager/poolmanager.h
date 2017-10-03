#pragma once

#include <QTime>

#include "control/pool/poolentity.h"
#include "exception/illegalargumentexception.h"
#include "sessiondata.h"

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
    PoolManager(Version programVersion, int selfPort = 0);
    ~PoolManager();

    void enable();
    void disable();

    void connectToUnconnectedNodes();
    void connectToNode(PoolEntityInfo& info);

    void removeNode(PoolEntityInfo& info);

    void start();
    bool isStarted() const { return started; }    

    SessionData* getSessionData() const { return data; }
    void setSessionData(SessionData* data);

signals:
    void sendDisconnected(QHostAddress address, int tcpPort);

private slots:
    void onNewConnection(QHostAddress address, int tcpPort);
    void onDisconnected(QHostAddress address, int tcpPort);

    void onOK(QUuid uuid);
    void onError(QUuid uuid, QString what);
    void onSendVersion(QUuid uuid, Version version);
    void onGetAvailableNode(QUuid uuid);

protected:
    void connectDispatcher();

private:
    bool tryConnect(PoolEntityInfo& info);

    void disconnectFromNode(PoolEntityInfo& info);
    void disconnectFromNodeWithoutNotification(PoolEntityInfo& info);

    void setStatusOfAllConnectedNodes(NodeStatus status);

    void disableManagers();
    void createSession();
    void startNodeInitialization(PoolEntityInfo& info);
    void sendLibraryListToNode(PoolEntityInfo& info);
    void sendArchitectureToNode(PoolEntityInfo& info);
    void markNodeInitialized(PoolEntityInfo& info);
    void sendAssignedNode(PoolEntityInfo& info, QUuid uuid);
    void sendCannotAssignNode(QUuid uuid);

    static const EntityType entityType = EntityType::Manager;

    SessionData* data;
    bool started;

    Version programVersion;
    QString currentSessionName;
};
