#include "poolentity.h"

PoolEntity::PoolEntity(int port) :
    transmitter(nullptr),
    selfPort(port)
{

}

PoolEntity::~PoolEntity()
{
    disableTransmitter();
}

void PoolEntity::setSelfPort(int port)
{
    if(isTransmitterEnabled())
    {
        throw Exception(tr("Cannot set the port while the transmitter is enabled."));
    }
    this->selfPort = port;
}

void PoolEntity::enableTransmitter()
{
    if(transmitter != nullptr)
        return;

    transmitter = new NetworkTransmitter(selfPort);
}

void PoolEntity::disableTransmitter()
{
    knownEntities.clear();
    outcomingRequests.clear();
    incomingRequests.clear();

    if(transmitter == nullptr)
        return;

    delete transmitter;
    transmitter = nullptr;
}

void PoolEntity::onDataReceived(QByteArray* data, QHostAddress address, int port)
{
    Command* command = new Command(data);

    if(dispatcher.isRequest(command->getType()))
        incomingRequests.append(CommandHistoryEntry(address, port, command->getType(), command->getUuid()));

    dispatcher.dispatchCommand(command);

    delete command;
}

CommandHistoryEntry& PoolEntity::getCommandHistoryEntry(QList<CommandHistoryEntry>& list, QUuid uuid)
{
    for(CommandHistoryEntry& entry: list)
    {
        if(entry.getUuid() == uuid)
            return entry;
    }

    throw IllegalArgumentException(tr("Cannot find the command history entry with uuid %1.")
                                   .arg(uuid.toString()));
}

void PoolEntity::sendRequest(QHostAddress address, int port, CommandType type, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, body);

    outcomingRequests.append(CommandHistoryEntry(address, port, command->getType(), command->getUuid()));
    transmitter->sendData(command->toByteArray(), address, port);    

    delete command;
}

void PoolEntity::sendUntrackedRequest(QHostAddress address, int port, CommandType type, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, body);
    transmitter->sendData(command->toByteArray(), address, port);

    delete command;
}

void PoolEntity::sendResponse(QHostAddress address, int port, CommandType type, QUuid uuid, QByteArray* body)
{
    Command* command = dispatcher.createCommand(type, uuid, body);
    transmitter->sendData(command->toByteArray(), address, port);

    delete command;
}

PoolEntityInfo& PoolEntity::removeRequestFromList(QList<CommandHistoryEntry>& list, QUuid uuid)
{
    CommandHistoryEntry& entry = getCommandHistoryEntry(list, uuid);

    PoolEntityInfo& info = getInfoByAddressAndPort(entry.getAddress(), entry.getPort());
    list.removeAll(entry);

    return info;
}

PoolEntityInfo& PoolEntity::getInfoByAddressAndPort(QHostAddress address, int port)
{
    for(PoolEntityInfo& info: knownEntities)
    {
        if(info.getAddress() == address && info.getTcpPort() == port)
            return info;
    }

    throw IllegalArgumentException(tr("Trying to access pool node at %1:%2 but node does not exist.")
                                   .arg(address.toString(), QString::number(port)));
}
