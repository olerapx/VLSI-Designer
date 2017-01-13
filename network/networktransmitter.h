#ifndef NETWORKTRANSMITTER_H
#define NETWORKTRANSMITTER_H

#include <QTcpServer>
#include <QMap>
#include <QThread>
#include <QDataStream>

#include "networkexception.h"
#include "tcpsocket.h"

/**
 * @brief Manages incoming and outcoming TCP connections to a given port.
 */
class NetworkTransmitter: public QObject
{
    Q_OBJECT

public:
    NetworkTransmitter();
    void init(int serverPort);

    int connectToHost (QHostAddress address, int port);
    void disconnectFromHost (QHostAddress address, int port);

    void sendData (QByteArray data, QHostAddress address, int port);

signals:
    void sendDataReceived(QByteArray data, QHostAddress address, int port);
    void sendLog (QString log);

private:
    QTcpServer* server;
    QMap <int, TcpSocket*> sockets;

    void addTcpSocket(QTcpSocket* qsocket);
    void removeTcpSocket (int descriptor);

    TcpSocket* findSocket (QHostAddress address, int port);

    void sendData (TcpSocket* socket, QByteArray data);

private slots:
    void on_newConnection();

    void on_socketDisconnected(int descriptor);
};

#endif // NETWORKTRANSMITTER_H
