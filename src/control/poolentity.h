#pragma once

#include "network/networktransmitter.h"

/**
 * @brief The PoolEntity class
 * Base class for pool objects such as manager and node.
 */
class PoolEntity : public QObject
{
public:
    PoolEntity(int selfPort = 0);
    virtual ~PoolEntity();

    int getSelfPort() { return selfPort; }
    void setSelfPort(int selfPort);

    void enableTransmitter();
    void disableTransmitter();

    bool isTransmitterEnabled() { return (transmitter != nullptr); }

protected:
    NetworkTransmitter* transmitter;

    int selfPort;
};
