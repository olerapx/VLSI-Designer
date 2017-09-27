#include "commandhistoryentry.h"

CommandHistoryEntry::CommandHistoryEntry(QHostAddress address, int port, CommandType type, QUuid uuid) :
    address(address),
    port(port),
    type(type),
    uuid(uuid)
{

}

bool CommandHistoryEntry::operator ==(const CommandHistoryEntry& other) const
{
    if(uuid == other.uuid)
        return true;

    return false;
}
