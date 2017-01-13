#include "tcpsocket.h"

TcpSocket::TcpSocket(QTcpSocket *socket)
{
    this->socket = socket;

    blockSize = 0;

    connect (socket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    connect (socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
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
    disconnect (socket, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    disconnect (socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));

    socket->deleteLater();

    sendDisconnected(socket->socketDescriptor());
}
