#include "commanddispatcher.h"

Command* CommandDispatcher::createCommand(CommandType type, QByteArray* body)
{
    return new Command(type, QUuid::createUuid(), body);
}

Command* CommandDispatcher::createCommand(CommandType type, QUuid uuid, QByteArray* body)
{
    return new Command(type, uuid, body);
}

void CommandDispatcher::dispatchCommand(Command* command)
{
    switch(command->getType())
    {
    case CommandType::GetVersion:
        sendGetVersion(command->getUuid());
        break;
    case CommandType::SendVersion:
        handleSendVersion(command);
        break;
    }
}

void CommandDispatcher::handleSendVersion(Command* command)
{
    QByteArray* array = command->getBody();
    QDataStream stream(array, QIODevice::ReadOnly);

    QString version;
    stream >> version;

    sendSendVersion(command->getUuid(), version);
}
