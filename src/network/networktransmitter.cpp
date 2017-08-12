#include "networktransmitter.h"

NetworkTransmitter::NetworkTransmitter(int serverPort)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkTransmitter::on_newConnection);

    if(!server->listen(QHostAddress::Any, serverPort))
    {
        delete server;
        throw NetworkException(tr("Cannot listen to port %1.").arg(serverPort));
    }
}

NetworkTransmitter::~NetworkTransmitter()
{
    disconnect(server, &QTcpServer::newConnection, this, &NetworkTransmitter::on_newConnection);
    delete server;
}

void NetworkTransmitter::on_newConnection()
{
    QTcpSocket* qsocket = server->nextPendingConnection();
    addTcpSocket(qsocket);

    sendLog(tr("Got a new connection from %1:%2.").arg(qsocket->peerAddress().toString(), qsocket->peerName()));
}

TcpSocket* NetworkTransmitter::addTcpSocket(QTcpSocket* qsocket)
{
    TcpSocket* socket = new TcpSocket(qsocket);
    sockets.append(socket);

    connect(socket, &TcpSocket::sendDataReceived, this, &NetworkTransmitter::sendDataReceived);
    connect(socket, &TcpSocket::sendDisconnected, this, &NetworkTransmitter::on_socketDisconnected);

    return socket;
}

void NetworkTransmitter::on_socketDisconnected(TcpSocket* socket)
{
    removeTcpSocket(socket);
}

void NetworkTransmitter::removeTcpSocket(TcpSocket* socket)
{
    sendLog(tr("Disconnected from %1:%2.").arg(socket->getSocket()->peerAddress().toString(),
                                                     socket->getSocket()->peerName()));

    disconnect(socket, &TcpSocket::sendDataReceived, this, &NetworkTransmitter::sendDataReceived);
    disconnect(socket, &TcpSocket::sendDisconnected, this, &NetworkTransmitter::on_socketDisconnected);

    delete socket->getSocket();
    delete socket;

    sockets.removeAll(socket);
}

TcpSocket* NetworkTransmitter::connectToHost(QHostAddress address, int port)
{
    if(findSocket(address, port) != nullptr)
        throw NetworkException(tr("Already connected to %1:%2.").arg(address.toString(), port));

    QTcpSocket* qsocket = new QTcpSocket(this);
    qsocket->connectToHost(address, port);
    qsocket->waitForConnected();

    if(qsocket->state() != QAbstractSocket::ConnectedState)
    {
        delete qsocket;
        throw NetworkException(tr("Cannot connect to %1:%2.").arg(address.toString(), port));
    }

    sendLog(tr("Connected to %1:%2.").arg(qsocket->peerAddress().toString(), qsocket->peerName()));

    return addTcpSocket(qsocket);
}

void NetworkTransmitter::disconnectFromHost(QHostAddress address, int port)
{
    TcpSocket* socket = findSocket (address, port);
    if(socket == nullptr)
        throw NetworkException(tr("Cannot disconnect from %1:%2: no socket found.").arg(address.toString(), port));

    socket->getSocket()->disconnectFromHost();
}

TcpSocket* NetworkTransmitter::findSocket(QHostAddress address, int port)
{
    for(TcpSocket* socket: sockets)
    {
        QTcpSocket* qsocket = socket->getSocket();
        if(qsocket->peerAddress() == address && qsocket->peerPort() == port)
            return socket;
    }
    return nullptr;
}

void NetworkTransmitter::sendData(QByteArray data, QHostAddress address, int port)
{
    TcpSocket* socket = findSocket(address, port);
    if(socket == nullptr)
        socket = connectToHost(address, port);

    sendData(socket, data);
}

void NetworkTransmitter::sendData(TcpSocket* socket, QByteArray data)
{
    QTcpSocket* qsocket = socket->getSocket();

    sendLog(tr("Sending data to %1:%2...").arg(qsocket->peerAddress().toString(), qsocket->peerName()));

    qint64 size = data.size();
    data.prepend(sizeof(qint64), ' ');

    QDataStream out (&data, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << size;

    qsocket->write(data.data(), data.size());
    qsocket->waitForBytesWritten();

    sendLog(tr("Data has been written to the buffer."));
}
