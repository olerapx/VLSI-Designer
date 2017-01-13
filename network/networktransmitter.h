#ifndef NETWORKTRANSMITTER_H
#define NETWORKTRANSMITTER_H

#include <QTcpServer>
#include <QMap>
#include <QThread>
#include <QDataStream>

#include "networkexception.h"
#include "tcpsocket.h"

class NetworkTransmitter: public QObject
{
    Q_OBJECT

public:
    NetworkTransmitter();
    void init(int serverPort);

    void sendData (QByteArray data, QHostAddress address, int port);

signals:
    void sendDataReceived(QByteArray data, QHostAddress address, int port);

private:
    QTcpServer* server;
    QMap <int, TcpSocket*> sockets;

    void sendData (QTcpSocket *socket, QByteArray data);

private slots:
    void on_newConnection();

    void on_socketDisconnected(int descriptor);
};

#endif // NETWORKTRANSMITTER_H
