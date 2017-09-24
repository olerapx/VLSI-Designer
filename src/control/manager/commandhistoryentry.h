#pragma once

#include <QHostAddress>

#include "control/command/command.h"

/**
 * @brief The CommandHistoryEntry class
 * Represents the command sent by pool manager on which a response is awaited.
 */
class CommandHistoryEntry
{
public:
    CommandHistoryEntry(QHostAddress address, int port, CommandType type, QUuid uuid);

    QHostAddress getAddress() { return address; }
    int getPort() { return port; }
    CommandType getType() { return type; }
    QUuid getUuid() { return uuid; }

    bool operator ==(const CommandHistoryEntry& other);

private:
    QHostAddress address;
    int port;
    CommandType type;
    QUuid uuid;
};
