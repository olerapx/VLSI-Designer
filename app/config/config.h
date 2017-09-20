#pragma once

#include "network/networkscanner.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The Config class
 * Represents the application configuration.
 */
class Config
{
public:
    Config(); //TODO: add class ConfigSerializer: read from config.json, init by default on fallback

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
    void setManagerTcpPort(int tcpPort);
    void setNodeTcpPort(int tcpPort);

private:
    void validatePort(int port);

    Mode mode;
    QHostAddress multicastAddress;

    QNetworkInterface networkInterface;

    int udpPort;
    int managerTcpPort;
    int nodeTcpPort;
};
