#pragma once

#include "network/networkscanner.h"

/**
 * @brief The Config class
 * Represents the application configuration.
 */
class Config
{
public:
    Config(); //TODO: read from config.json, init by default on fallback

    Mode getMode() { return mode; }
    QHostAddress getMulticastAddress() { return multicastAddress; }

    void setMode(Mode mode);
    void setMulticastAddress(QHostAddress multicastAddress);

    QNetworkInterface getNetworkInterface() { return networkInterface; }
    void setNetworkInterface(QNetworkInterface interface);

    int getUdpPort() { return udpPort; }
    int getTcpPort() { return tcpPort; }

    void setUdpPort(int udpPort);
    void setTcpPort(int tcpPort);

private:
    Mode mode;
    QHostAddress multicastAddress;

    QNetworkInterface networkInterface;

    int udpPort;
    int tcpPort;
};
