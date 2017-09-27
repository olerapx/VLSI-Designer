#include "commanddispatcher.h"

Command* CommandDispatcher::createCommand(CommandType type, QByteArray* body)
{
    return new Command(type, QUuid::createUuid(), body);
}

Command* CommandDispatcher::createCommand(CommandType type, QUuid uuid, QByteArray* body)
{
    return new Command(type, uuid, body);
}

bool CommandDispatcher::isRequest(CommandType type)
{
    switch(type)
    {
    case CommandType::Identify:
        return true;
    case CommandType::GetVersion:
        return true;
    case CommandType::SendVersion:
        return false;
    default:
        return true;
    }
}

void CommandDispatcher::dispatchCommand(Command* command)
{
    switch(command->getType())
    {
    case CommandType::Identify:
        handleIdentify(command);
        break;
    case CommandType::GetVersion:
        sendGetVersion(command->getUuid());
        break;
    case CommandType::SendVersion:
        handleSendVersion(command);
        break;
    }
}

void CommandDispatcher::handleIdentify(Command* command)
{
    QByteArray* array = command->getBody();
    QDataStream stream(array, QIODevice::ReadOnly);

    qint32 type;
    stream >> type;

    sendIdentify(command->getUuid(), static_cast<EntityType>(type));
}

void CommandDispatcher::handleSendVersion(Command* command)
{
    QByteArray* array = command->getBody();
    QDataStream stream(array, QIODevice::ReadOnly);

    QString version;
    stream >> version;

    sendSendVersion(command->getUuid(), Version(version));
}
