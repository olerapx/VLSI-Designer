#include "tcpsocket.h"

TcpSocket::TcpSocket(QTcpSocket *socket)
{
    this->socket = socket;

    blockSize = 0;

    connect (socket, &QTcpSocket::readyRead, this, &TcpSocket::on_readyRead);
    connect (socket, &QTcpSocket::disconnected, this, &TcpSocket::on_disconnected);
}

void TcpSocket::on_readyRead()
{
    QDataStream in(socket);

    if (blockSize == 0)
    {
        if (socket->bytesAvailable() < (qint64) sizeof(qint64))
            return;

        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize)
        return;

    QByteArray data;
    data.resize(blockSize);
    in.readRawData(data.data(), blockSize);

    sendDataReceived(data, socket->peerAddress(), socket->peerPort());

    blockSize = 0;
}

void TcpSocket::on_disconnected()
{
    connect (socket, &QTcpSocket::readyRead, this, &TcpSocket::on_readyRead);
    connect (socket, &QTcpSocket::disconnected, this, &TcpSocket::on_disconnected);

    sendDisconnected(this);
}
