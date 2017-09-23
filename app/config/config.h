#pragma once

#include <QDir>

#include "network/networkscanner.h"
#include "exception/illegalargumentexception.h"

class ConfigBuilder;

/**
 * @brief The Config class
 * Represents the application configuration.
 */
class Config
{
    friend class ConfigBuilder;

public:
    Mode getMode() { return mode; }
    QHostAddress getMulticastAddress() { return multicastAddress; }

    void setMode(Mode mode);
    void setMulticastAddress(QHostAddress multicastAddress);

    QNetworkInterface getNetworkInterface() { return networkInterface; }
    void setNetworkInterface(QNetworkInterface interface);

    int& getUdpPort() { return udpPort; }
    int& getManagerTcpPort() { return managerTcpPort; }
    int& getNodeTcpPort() { return nodeTcpPort; }

    void setUdpPort(int udpPort);
    void setTcpPorts(int managerTcpPort, int nodeTcpPort);

    QString getLibrariesPath();
    void setLibrariesPath(QString path);

private:
    Config() {}

    void validatePort(int port);

    Mode mode;
    QHostAddress multicastAddress;

    QNetworkInterface networkInterface;

    int udpPort;
    int managerTcpPort;
    int nodeTcpPort;

    QString librariesPath;
};
