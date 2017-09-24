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
    PoolManagerInfo(QHostAddress address, int tcpPort);

    QHostAddress getAddress() const { return address; }
    int getTcpPort() const { return tcpPort; }

private:
    QHostAddress address;
    int tcpPort;
};
