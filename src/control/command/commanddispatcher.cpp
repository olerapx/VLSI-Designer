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
    case CommandType::OK:
        return false;
    case CommandType::Error:
        return false;
    case CommandType::Identify:
        return true;
    case CommandType::GetVersion:
        return true;
    case CommandType::SendVersion:
        return false;
    case CommandType::DisableManager:
        return true;
    case CommandType::SendSessionDirectoryName:
        return true;
    case CommandType::SendLibraryList:
        return true;
    case CommandType::SendArchitecture:
        return true;
    case CommandType::Assign:
        return true;
    default:
        return true;
    }
}

void CommandDispatcher::dispatchCommand(Command* command)
{
    switch(command->getType())
    {
    case CommandType::OK:
        sendOK(command->getUuid());
        break;
    case CommandType::Error:
        handleError(command);
        break;
    case CommandType::Identify:
        handleIdentify(command);
        break;
    case CommandType::GetVersion:
        sendGetVersion(command->getUuid());
        break;
    case CommandType::SendVersion:
        handleSendVersion(command);
        break;
    case CommandType::DisableManager:
        sendDisableManager(command->getUuid());
        break;
    case CommandType::SendSessionDirectoryName:
        handleSendSessionDirectoryName(command);
        break;
    case CommandType::SendLibraryList:
        handleSendLibraryList(command);
        break;
    case CommandType::SendArchitecture:
        handleSendArchitecture(command);
        break;
    case CommandType::Assign:
        sendAssign(command->getUuid());
        break;
    }
}

void CommandDispatcher::handleError(Command* command)
{
    QByteArray* array = command->getBody();
    QString what(*array);

    sendError(command->getUuid(), what);
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

void CommandDispatcher::handleSendSessionDirectoryName(Command* command)
{
    QByteArray* array = command->getBody();
    QString name(*array);

    sendSendSessionDirectoryName(command->getUuid(), name);
}

void CommandDispatcher::handleSendLibraryList(Command* command)
{
    QByteArray* body = command->getBody();
    QDataStream stream(body, QIODevice::ReadOnly);

    qint32 size;
    stream >> size;

    BinarySerializer serializer;

    QList<Library*> libraries;
    for(int i=0; i<size; i++)
    {
        quint32 bodyLength;
        stream >> bodyLength;

        char* rawBody = new char[bodyLength];
        stream.readRawData(rawBody, bodyLength);

        QByteArray libraryByteArray(QByteArray::fromRawData(rawBody, bodyLength));

        Library* library = dynamic_cast<Library*>(serializer.deserialize(libraryByteArray));

        libraries.append(library);
    }

    sendSendLibraryList(command->getUuid(), libraries);
}

void CommandDispatcher::handleSendArchitecture(Command* command)
{
    QByteArray* body = command->getBody();

    BinarySerializer serializer;
    Architecture* architecture = dynamic_cast<Architecture*>(serializer.deserialize(*body));

    sendSendArchitecture(command->getUuid(), architecture);
}
