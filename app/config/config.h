#pragma once

#include "network/networkscanner.h"

/**
 * @brief The Config class
 * Represents the application configuration.
 */
class Config
{
public:
    Config();

    Mode getMode() { return mode; }
    QHostAddress getMulticastAddress() { return multicastAddress; }

    int getPort() { return port; }

    void setMode(Mode mode);
    void setMulticastAddress(QHostAddress multicastAddress);

    void setPort(int port);

private:
    Mode mode;
    QHostAddress multicastAddress;

    int port;
};
