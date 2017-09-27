#pragma once

#include <QString>
#include <QHostAddress>

#include "datamodels/version/version.h"

enum class NodeStatus
{
    Ready,
    Unconnected,
    NotResponding,
    Connecting,
    Initialization,
    Assigned,
    Working,
    Error,
    Incompatible
};

/**
 * @brief The PoolEntityInfo class
 * Encapsulates the information and current status of pool node or pool manager.
 */
class PoolEntityInfo
{
public:
    PoolEntityInfo(QString hostName, QHostAddress address, int tcpPort);

    QString getHostName() const { return hostName; }

    NodeStatus getStatus() const { return status; }
    void setStatus(NodeStatus status);

    Version getProgramVersion() { return programVersion; }
    void setProgramVersion(Version version);

    QHostAddress getAddress() const { return address; }

    int getTcpPort() const { return tcpPort; }

    bool operator ==(const PoolEntityInfo& other) const;

private:
    QString hostName;
    NodeStatus status;
    Version programVersion;
    QHostAddress address;

    int tcpPort;
};
