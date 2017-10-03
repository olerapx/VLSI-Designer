#pragma once

#include <QUuid>
#include <QDataStream>

enum class CommandType
{
    OK,
    Error,
    Identify,
    GetVersion,
    SendVersion,
    DisableManager,
    SendSessionDirectoryName,
    SendLibraryList,
    SendArchitecture,
    GetAvailableNode,
    Assign,
    SendAssignedNode,
    SendScheme,
    SendGrid
};

/**
 * @brief The Command class
 * Represents a command transmitted over the network between manager and nodes.
 * The command body is deleted in destructor.
 */
class Command
{
public:
    /**
     * @brief Command
     * Constructs the object from the specified parameters.
     * @param type - command type.
     * @param uuid - command unique idenfier.
     * @param body - command body. Shallow-copied to the object.
     */
    Command(CommandType type, QUuid uuid, QByteArray* body);

    /**
     * @brief Command
     * Deserializes the object from the byte array obtainet from the network.
     * The consistency checking is not performed. The array will not be deleted.
     * @param array
     */
    Command(QByteArray* array);

    ~Command();

    CommandType getType() const { return type; }
    QUuid getUuid() const { return uuid; }
    QByteArray* getBody() const { return body; }

    /**
     * @brief toByteArray
     * Serializes the object to a byte array to transmit over the network.
     * @return
     */
    QByteArray* toByteArray();

    bool operator ==(const Command& other) const;

private:
    CommandType type;
    QUuid uuid;
    QByteArray* body;
};
