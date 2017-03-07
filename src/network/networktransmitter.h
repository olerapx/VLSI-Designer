#ifndef NETWORKTRANSMITTER_H
#define NETWORKTRANSMITTER_H

#include <QTcpServer>
#include <QList>
#include <QThread>
#include <QDataStream>

#include "src/network/networkexception.h"
#include "tcpsocket.h"

/**
 * @brief Manages incoming and outcoming TCP connections on a given port.
 *
 * The NetworkTransmitter represents a TCP server.
 * This class can be used to exchange the data between known hosts using the protocol.
 * The data size can be up to 2GiB.
 * The server will constantly listen to a specified server port.
 * Note that there can be only one server listening to each port or the exception will be raised.
 */
class NetworkTransmitter: public QObject
{
    Q_OBJECT

public:
    NetworkTransmitter(int serverPort);
    ~NetworkTransmitter();

    /**
     * @brief Connects to the specified host. Called automatically by sendData() if needed.
     * @param address
     * @param port
     * @return a created socket.
     */
    TcpSocket* connectToHost(QHostAddress address, int port);
    void disconnectFromHost (QHostAddress address, int port);

    /**
     * @brief Sends the specified byte array to the host.
     *
     * The data is placed to TCP buffer almost immediately and transmitted asyncronously.
     * @param data
     * @param address
     * @param port
     */
    void sendData (QByteArray data, QHostAddress address, int port);

signals:
    /**
     * @brief Emits when a full data is received.
     * @param data - a received data.
     * @param address - a sender address.
     * @param port - a sender port.
     */
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
