#include "tcpsocket.h"

TcpSocket::TcpSocket(QTcpSocket* socket)
{
    this->socket = socket;

    blockSize = 0;

    connect(socket, &QTcpSocket::readyRead, this, &TcpSocket::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TcpSocket::onDisconnected);
}

void TcpSocket::onReadyRead()
{
    QDataStream in(socket);

    if(blockSize == 0)
    {
        if(socket->bytesAvailable() < (qint64) sizeof(qint64))
            return;

        in >> blockSize;
    }

    if(socket->bytesAvailable() < blockSize)
        return;

    QByteArray* data = new QByteArray();
    data->resize(blockSize);
    in.readRawData(data->data(), blockSize);

    sendDataReceived(data, socket->peerAddress(), socket->peerPort());

    blockSize = 0;
}

void TcpSocket::onDisconnected()
{
    disconnect(socket, &QTcpSocket::readyRead, this, &TcpSocket::onReadyRead);
    disconnect(socket, &QTcpSocket::disconnected, this, &TcpSocket::onDisconnected);

    sendDisconnected(this, socket->peerAddress(), socket->peerPort());
}
