#pragma once

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>

/**
 * @brief Wraps QTcpSocket and current block size to provide receiving full data.
 * Does not allocate or delete a QTcpSocket object.
 */
class TcpSocket : public QObject
{
    Q_OBJECT

public:
    TcpSocket(QTcpSocket* socket);

    QTcpSocket* getSocket() const { return this->socket; }

signals:
    /**
     * @brief sendDataReceived
     * Emits when the socket finishes receiving a data.
     * @param data - the received data.
     * @param address - the sender's address.
     * @param port - the sender's port.
     */
    void sendDataReceived(QByteArray* data, QHostAddress address, int port);

    /**
     * @brief sendDisconnected
     * Emits when the socket is disconnected.
     * @param socket - this.
     */
    void sendDisconnected(TcpSocket* socket);

private:
    QTcpSocket* socket;
    qint64 blockSize;

private slots:
    void onReadyRead();
    void onDisconnected();
};
