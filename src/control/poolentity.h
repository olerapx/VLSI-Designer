#pragma once

#include "network/networktransmitter.h"
#include "control/command/commanddispatcher.h"

/**
 * @brief The PoolEntity class
 * Base class for pool objects such as manager and node.
 */
class PoolEntity : public QObject
{
public:
    PoolEntity(int selfPort = 0);
    virtual ~PoolEntity();

    int getSelfPort() const { return selfPort; }
    void setSelfPort(int selfPort);

    void enableTransmitter();
    void disableTransmitter();

    bool isTransmitterEnabled() { return (transmitter != nullptr); }

protected:
    NetworkTransmitter* transmitter;
    CommandDispatcher dispatcher;

    int selfPort;

    virtual void connectDispatcher() = 0;
};
