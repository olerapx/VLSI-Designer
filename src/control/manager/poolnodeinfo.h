#pragma once

#include <QString>
#include <QHostAddress>

enum class NodeStatus
{
    Ready,
    Unconnected,
    NotResponding,
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

    QString getHostName() { return hostName; }
    NodeStatus getStatus() { return status; }
    void setStatus(NodeStatus status);
    double getProgramVersion() { return programVersion; }
    QHostAddress getAddress() { return address; }
    int getTcpPort() { return tcpPort; }

private:
    QString hostName;
    NodeStatus status;
    double programVersion;
    QHostAddress address;

    int tcpPort;
};
