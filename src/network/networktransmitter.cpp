#include "networktransmitter.h"

NetworkTransmitter::NetworkTransmitter(int port)
{
    connect(&server, &QTcpServer::newConnection, this, &NetworkTransmitter::onNewConnection);

    if(!server.listen(QHostAddress::Any, port))
        throw NetworkException(tr("Cannot listen to port %1.").arg(port));
}

NetworkTransmitter::~NetworkTransmitter()
{
    for(TcpSocket* socket: sockets)
        removeTcpSocket(socket);

    disconnect(&server, &QTcpServer::newConnection, this, &NetworkTransmitter::onNewConnection);
}

void NetworkTransmitter::onNewConnection()
{
    QTcpSocket* qsocket = server.nextPendingConnection();
    addTcpSocket(qsocket);

    sendNewConnection(qsocket->peerName(), qsocket->peerAddress(), qsocket->peerPort());
}

TcpSocket* NetworkTransmitter::addTcpSocket(QTcpSocket* qsocket)
{
    TcpSocket* socket = new TcpSocket(qsocket);
    sockets.append(socket);

    connect(socket, &TcpSocket::sendDataReceived, this, &NetworkTransmitter::sendDataReceived);
    connect(socket, &TcpSocket::sendDisconnected, this, &NetworkTransmitter::onSocketDisconnected);

    return socket;
}

void NetworkTransmitter::onSocketDisconnected(TcpSocket* socket)
{
    removeTcpSocket(socket);
}

void NetworkTransmitter::removeTcpSocket(TcpSocket* socket)
{
    sendLog(tr("Disconnected from %1:%2.").arg(socket->getSocket()->peerAddress().toString(),
                                                     socket->getSocket()->peerName()));

    sendDisconnected(socket->getSocket()->peerName(), socket->getSocket()->peerAddress(), socket->getSocket()->peerPort());

    disconnect(socket, &TcpSocket::sendDataReceived, this, &NetworkTransmitter::sendDataReceived);
    disconnect(socket, &TcpSocket::sendDisconnected, this, &NetworkTransmitter::onSocketDisconnected);

    sockets.removeAll(socket);

    delete socket->getSocket();
    delete socket;   
}

TcpSocket* NetworkTransmitter::connectToHost(QHostAddress address, int port)
{
    TcpSocket* existingSocket = findSocket(address, port);

    if(existingSocket != nullptr)
        return existingSocket;

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
    TcpSocket* socket = findSocket(address, port);
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

void NetworkTransmitter::sendData(QByteArray* data, QHostAddress address, int port)
{
    TcpSocket* socket = findSocket(address, port);
    if(socket == nullptr)
        socket = connectToHost(address, port);

    sendData(socket, data);
}

void NetworkTransmitter::sendData(TcpSocket* socket, QByteArray* data)
{
    QTcpSocket* qsocket = socket->getSocket();

    sendLog(tr("Sending data to %1:%2...").arg(qsocket->peerAddress().toString(), qsocket->peerName()));

    qint64 size = data->size();
    data->prepend(sizeof(qint64), ' ');

    QDataStream out (data, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << size;

    qsocket->write(data->data(), data->size());
    qsocket->waitForBytesWritten();

    sendLog(tr("Data has been written to the buffer."));
}
