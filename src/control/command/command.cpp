#include "command.h"

Command::Command(CommandType type, QUuid uuid, QByteArray* body) :
    type(type),
    uuid(uuid),
    body(body)
{

}

Command::Command(QByteArray* array)
{
    QDataStream stream(array, QIODevice::ReadOnly);

    qint32 type;
    stream >> type;

    this->type = static_cast<CommandType>(type);

    stream >> this->uuid;

    quint32 bodyLength;
    stream >> bodyLength;

    char* rawBody = new char[bodyLength];
    stream.readRawData(rawBody, bodyLength);

    body = new QByteArray(QByteArray::fromRawData(rawBody, bodyLength));
}

Command::~Command()
{
    delete body;
}

QByteArray* Command::toByteArray()
{
    QByteArray* array = new QByteArray();

    QDataStream stream(array, QIODevice::WriteOnly);

    stream << static_cast<qint32>(type);
    stream << uuid;

    stream << static_cast<quint32>(body->size());
    stream.writeRawData(body->constData(), body->size());
    return array;
}

bool Command::operator ==(const Command& other) const
{
    if(type == other.type && uuid == other.uuid && *body == *(other.body))
        return true;

    return false;
}
