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

    QHostAddress getAddress() const { return address; }
    int getPort() const { return port; }
    CommandType getType() const { return type; }
    QUuid getUuid() const { return uuid; }

    bool operator ==(const CommandHistoryEntry& other) const;

private:
    QHostAddress address;
    int port;
    CommandType type;
    QUuid uuid;
};
