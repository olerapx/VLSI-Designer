#ifndef NETWORKTRANSMITTER_H
#define NETWORKTRANSMITTER_H

#include <QTcpServer>
#include <QList>
#include <QThread>
#include <QDataStream>

#include "networkexception.h"
#include "tcpsocket.h"

/**
 * @brief Manages incoming and outcoming TCP connections on a given port.
 */
class NetworkTransmitter: public QObject
{
    Q_OBJECT

public:
    NetworkTransmitter(int serverPort);
    ~NetworkTransmitter();

    TcpSocket* connectToHost(QHostAddress address, int port);
    void disconnectFromHost (QHostAddress address, int port);

    void sendData (QByteArray data, QHostAddress address, int port);

signals:
    void sendDataReceived(QByteArray data, QHostAddress address, int port);
    void sendLog (QString log);

private:
    QTcpServer* server;
    QList <TcpSocket*> sockets;

    TcpSocket* addTcpSocket(QTcpSocket* qsocket);
    void removeTcpSocket (TcpSocket *socket);

    TcpSocket* findSocket (QHostAddress address, int port);

    void sendData (TcpSocket* socket, QByteArray data);

private slots:
    void on_newConnection();

    void on_socketDisconnected(TcpSocket* socket);
};

#endif // NETWORKTRANSMITTER_H
