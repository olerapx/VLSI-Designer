#pragma once

#include <QHostAddress>

#include "command.h"
#include "control/pool/entitytype.h"
#include "datamodels/version/version.h"

/**
 * @brief The CommandDispatcher class
 * Handles incoming and outcoming commands.
 *
 * The command dispatcher is used to disassemble an incoming command
 * and to send an appropriate signal with command data.
 * It is also used to create a command from the given type and body using a randomly-generated UUID.
 */
class CommandDispatcher : public QObject
{
    Q_OBJECT

public:
    CommandDispatcher() {}

    Command* createCommand(CommandType type, QByteArray* body);
    Command* createCommand(CommandType type, QUuid uuid, QByteArray* body);

    static bool isRequest(CommandType type);

    void dispatchCommand(Command* command);    

signals:
    void sendIdentify(QUuid uuid, EntityType type);
    void sendGetVersion(QUuid uuid);
    void sendSendVersion(QUuid uuid, Version version);

private:
    void handleIdentify(Command* command);
    void handleSendVersion(Command* command);
};
