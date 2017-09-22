#pragma once

#include <QTcpServer>
#include <QList>
#include <QThread>
#include <QDataStream>

#include "network/networkexception.h"
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
class NetworkTransmitter : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief NetworkTransmitter
     * @param port - the port the transmitter will be listen to.
     * @throw NetworkException
     */
    NetworkTransmitter(int port);
    ~NetworkTransmitter();

    /**
     * @brief Connects to the specified host. Called automatically by sendData() if needed.
     * @param address
     * @param port
     * @return the created socket.
     * @throw NetworkException
     */
    TcpSocket* connectToHost(QHostAddress address, int port);

    /**
     * @brief disconnectFromHost
     * Disconnects the transmitter from the specified host.
     * @param address
     * @param port
     * @throws NetworkException
     */
    void disconnectFromHost(QHostAddress address, int port);

    /**
     * @brief sendData
     * Sends the specified byte array to the host.
     * The data is placed to TCP buffer almost immediately and transmitted asyncronously.
     * @param data
     * @param address
     * @param port
     */
    void sendData(QByteArray* data, QHostAddress address, int port);

signals:
    /**
     * @brief sendDataReceived
     * Emits when a full data is received.
     * @param data - the received data.
     * @param address - the sender's address.
     * @param port - the sender's port.
     */
    void sendDataReceived(QByteArray* data, QHostAddress address, int port);
    void sendLog(QString log);

    /**
     * @brief sendNewConnection
     * Emits when a new connection is established.
     */
    void sendNewConnection(QString hostName, QHostAddress address, int tcpPort);

    /**
     * @brief sendDisconnected
     * Emits when a connection is over.
     */
    void sendDisconnected(QString hostName, QHostAddress address, int tcpPort);

private slots:
    void onNewConnection();
    void onSocketDisconnected(TcpSocket* socket);

private:
    TcpSocket* addTcpSocket(QTcpSocket* qsocket);
    void removeTcpSocket(TcpSocket* socket);

    TcpSocket* findSocket(QHostAddress address, int port);

    void sendData(TcpSocket* socket, QByteArray* data);

    QTcpServer server;
    QList<TcpSocket*> sockets;
};
