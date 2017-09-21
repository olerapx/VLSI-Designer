#pragma once

#include <QString>
#include <QHostAddress>

/**
 * @brief The PoolManagerInfo class
 * Encapsulates the information about pool manager hold by pool node.
 */
class PoolManagerInfo
{
public:
    PoolManagerInfo(QString hostName, QHostAddress address, int tcpPort);

    QString getHostName() const { return hostName; }
    QHostAddress getAddress() const { return address; }
    int getTcpPort() const { return tcpPort; }

private:
    QString hostName;
    QHostAddress address;
    int tcpPort;
};
