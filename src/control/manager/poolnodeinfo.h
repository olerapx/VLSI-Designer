#pragma once

#include <QString>
#include <QHostAddress>

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
 * @brief The PoolNodeInfo class
 * Encapsulates the information and current status of pool node hold by pool manager.
 */
class PoolNodeInfo
{
public:
    PoolNodeInfo(QString hostName, QHostAddress address, int tcpPort);

    QString getHostName() const { return hostName; }
    NodeStatus getStatus() const { return status; }
    void setStatus(NodeStatus status);
    double getProgramVersion() { return programVersion; }
    QHostAddress getAddress() const { return address; }
    int getTcpPort() const { return tcpPort; }

    bool operator ==(const PoolNodeInfo& other);

private:
    QString hostName;
    NodeStatus status;
    double programVersion;
    QHostAddress address;

    int tcpPort;
};
