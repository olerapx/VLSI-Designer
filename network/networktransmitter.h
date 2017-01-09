#ifndef NETWORKTRANSMITTER_H
#define NETWORKTRANSMITTER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

#include "networkexception.h"

class NetworkTransmitter: public QObject
{
    Q_OBJECT

public:
    NetworkTransmitter();

    void init(int serverPort);

    void sendData (QByteArray data, QHostAddress address, int port);

signals:
    void dataReceived(QByteArray data, QHostAddress address, int port);

private:
    QTcpServer* server;
    QMap <int, QTcpSocket*> sockets;

    void sendData (QTcpSocket *socket, QByteArray data);

private slots:
    void on_newConnection();

    void on_socketReadyRead();
    void on_socketDisconnected();
};

#endif // NETWORKTRANSMITTER_H
