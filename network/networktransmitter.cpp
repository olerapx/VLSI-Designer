#include "networktransmitter.h"

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
    QTcpSocket* qsocket = server->nextPendingConnection();
    addTcpSocket(qsocket);

    sendLog (QString("New connection from %1:%2.").arg(qsocket->peerAddress().toString(), qsocket->peerName()));
}

void NetworkTransmitter::addTcpSocket(QTcpSocket *qsocket)
{
    TcpSocket* socket = new TcpSocket(qsocket);
    sockets.insert(socket->getSocket()->socketDescriptor(), socket);

    connect (socket, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)), this, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)));
    connect (socket, SIGNAL(sendDisconnected(int)), this, SLOT(on_socketDisconnected(int)));
}

void NetworkTransmitter::on_socketDisconnected(int descriptor)
{
    removeTcpSocket(descriptor);
}

void NetworkTransmitter::removeTcpSocket(int descriptor)
{
    TcpSocket* socket = sockets[descriptor];

    sendLog (QString("Disconnected from %1:%2.").arg(socket->getSocket()->peerAddress().toString(), socket->getSocket()->peerName()));

    disconnect (socket, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)), this, SIGNAL(sendDataReceived(QByteArray,QHostAddress,int)));
    disconnect (socket, SIGNAL(sendDisconnected(int)), this, SLOT(on_socketDisconnected(int)));

    delete socket;

    sockets.remove(descriptor);
}

int NetworkTransmitter::connectToHost(QHostAddress address, int port)
{
    if (findSocket(address, port) != nullptr)
        throw NetworkException ("Already connected.");

    QTcpSocket* qsocket = new QTcpSocket(this);
    qsocket->connectToHost(address, port);
    qsocket->waitForConnected();

    if (qsocket->state()!=QAbstractSocket::ConnectedState)
    {
        delete qsocket;
        throw NetworkException (QString("Can not connect to %1:%2.").arg(address.toString(), port));
    }

    sendLog (QString("Connected to %1:%2.").arg(qsocket->peerAddress().toString(), qsocket->peerName()));

    addTcpSocket(qsocket);
    return qsocket->socketDescriptor();
}

void NetworkTransmitter::disconnectFromHost (QHostAddress address, int port)
{
    TcpSocket* socket = findSocket (address, port);
    if (socket == nullptr)
        throw NetworkException (QString("Can not disconnect from %1:%2: no socket found.").arg(address.toString(), port));

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
    {
        int descriptor = connectToHost(address, port);
        socket = sockets[descriptor];
    }

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

    sendLog (QString("Data has been sent."));
}
