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
    TcpSocket* socket = new TcpSocket(server->nextPendingConnection());
    sockets.insert(socket->getSocket()->socketDescriptor(), socket);

    connect (socket, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)), this, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)));
    connect (socket, SIGNAL(sendDisconnected(int)), this, SLOT(on_socketDisconnected(int)));
}

void NetworkTransmitter::on_socketDisconnected(int descriptor)
{
    TcpSocket* socket = sockets[descriptor];
    delete socket;

    sockets.remove(descriptor);
}

// TODO: don't use temp sockets
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
