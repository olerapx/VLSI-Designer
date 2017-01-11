#include "networktransmitter.h"
#include <iostream>

NetworkTransmitter::NetworkTransmitter()
{

}

void NetworkTransmitter::init(int serverPort)
{
    server = new QTcpServer(this);
    connect (server, SIGNAL(newConnection()), this, SLOT(on_newConnection()));

    if (!server->listen(QHostAddress::Any, serverPort))
        throw NetworkException (QString("Cannot listen to port %1.").arg(serverPort));
}

void NetworkTransmitter::on_newConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    sockets.insert(socket->socketDescriptor(), socket);

    connect (socket, SIGNAL(readyRead()), this, SLOT(on_socketReadyRead()));
    connect (socket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
}

void NetworkTransmitter::on_socketDisconnected()
{
    QTcpSocket* socket = static_cast<QTcpSocket*> (sender());

    sockets.remove(socket->socketDescriptor());
    disconnect (socket, SIGNAL(readyRead()), this, SLOT(on_socketReadyRead()));
    disconnect (socket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));

    socket->deleteLater();
}

void NetworkTransmitter::on_socketReadyRead()
{
    QTcpSocket* socket = static_cast<QTcpSocket*> (sender());

    QDataStream in(socket);

    if (size == 0)
    {
        if (socket->bytesAvailable() < (qint64) sizeof(qint64))
            return;
        in >> size;
    }

    if (socket->bytesAvailable() < size)
        return;

    QByteArray data;
    data.resize(size);
    in.readRawData(data.data(), size);

    dataReceived(data, socket->peerAddress(), socket->peerPort());

    size = 0;
}

void NetworkTransmitter::sendData(QByteArray data, QHostAddress address, int port)
{
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(address, port);

    sendData(socket, data);

    socket->disconnectFromHost();
    delete socket;
}

void NetworkTransmitter::sendData(QTcpSocket* socket, QByteArray data)
{
    qint64 size = data.size();
    data.prepend(sizeof(qint64), ' ');

    QDataStream out (&data, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << size;

    socket->write(data.data(), data.size());
    socket->waitForBytesWritten();
}
