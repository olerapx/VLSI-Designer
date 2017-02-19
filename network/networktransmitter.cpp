#include "networktransmitter.h"

NetworkTransmitter::NetworkTransmitter(int serverPort)
{
    server = new QTcpServer(this);
    connect (server, SIGNAL(newConnection()), this, SLOT(on_newConnection()));

    if (!server->listen(QHostAddress::Any, serverPort))
        throw NetworkException (QString("Cannot listen to port %1.").arg(serverPort));
}

NetworkTransmitter::~NetworkTransmitter()
{
    disconnect (server, SIGNAL(newConnection()), this, SLOT(on_newConnection()));
    delete server;
}

void NetworkTransmitter::on_newConnection()
{
    QTcpSocket* qsocket = server->nextPendingConnection();
    addTcpSocket(qsocket);

    sendLog (QString("Got a new connection from %1:%2.").arg(qsocket->peerAddress().toString(), qsocket->peerName()));
}

TcpSocket* NetworkTransmitter::addTcpSocket(QTcpSocket *qsocket)
{
    TcpSocket* socket = new TcpSocket(qsocket);
    sockets.append(socket);

    connect (socket, SIGNAL(sendDataReceived(QByteArray, QHostAddress, int)), this, SIGNAL(sendDataReceived(QByteArray, QHostAddress, int)));
    connect (socket, SIGNAL(sendDisconnected(TcpSocket*)), this, SLOT(on_socketDisconnected(TcpSocket*)));

    return socket;
}

void NetworkTransmitter::on_socketDisconnected(TcpSocket* socket)
{
    removeTcpSocket(socket);
}

void NetworkTransmitter::removeTcpSocket(TcpSocket* socket)
{
    sendLog (QString("Disconnected from %1:%2.").arg(socket->getSocket()->peerAddress().toString(), socket->getSocket()->peerName()));

    disconnect (socket, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)), this, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)));
    disconnect (socket, SIGNAL(sendDisconnected(TcpSocket*)), this, SLOT(on_socketDisconnected(TcpSocket*)));

    delete socket->getSocket();
    delete socket;

    sockets.removeAll(socket);
}

TcpSocket* NetworkTransmitter::connectToHost(QHostAddress address, int port)
{
    if (findSocket(address, port) != nullptr)
        throw NetworkException (QString("Already connected to %1:%2.").arg(address.toString(), port));

    QTcpSocket* qsocket = new QTcpSocket(this);
    qsocket->connectToHost(address, port);
    qsocket->waitForConnected();

    if (qsocket->state() != QAbstractSocket::ConnectedState)
    {
        delete qsocket;
        throw NetworkException (QString("Cannot connect to %1:%2.").arg(address.toString(), port));
    }

    sendLog (QString("Connected to %1:%2.").arg(qsocket->peerAddress().toString(), qsocket->peerName()));

    return addTcpSocket(qsocket);
}

void NetworkTransmitter::disconnectFromHost (QHostAddress address, int port)
{
    TcpSocket* socket = findSocket (address, port);
    if (socket == nullptr)
        throw NetworkException (QString("Cannot disconnect from %1:%2: no socket found.").arg(address.toString(), port));

    socket->getSocket()->disconnectFromHost();
}

TcpSocket* NetworkTransmitter::findSocket(QHostAddress address, int port)
{
    foreach (TcpSocket* socket, sockets)
    {
        QTcpSocket* qsocket = socket->getSocket();
        if (qsocket->peerAddress() == address && qsocket->peerPort() == port)
            return socket;
    }
    return nullptr;
}

void NetworkTransmitter::sendData(QByteArray data, QHostAddress address, int port)
{
    TcpSocket* socket = findSocket(address, port);
    if (socket == nullptr)
        socket = connectToHost(address, port);

    sendData(socket, data);
}

void NetworkTransmitter::sendData(TcpSocket* socket, QByteArray data)
{
    QTcpSocket* qsocket = socket->getSocket();

    sendLog (QString("Sending data to %1:%2...").arg(qsocket->peerAddress().toString(), qsocket->peerName()));

    qint64 size = data.size();
    data.prepend(sizeof(qint64), ' ');

    QDataStream out (&data, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << size;

    qsocket->write(data.data(), data.size());
    qsocket->waitForBytesWritten();

    sendLog (QString("Data has been written to the buffer."));
}
