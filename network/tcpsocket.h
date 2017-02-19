#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>

/**
 * @brief Wraps QTcpSocket and current block size to provide receiving full data.
 * Does not allocate or delete a QTcpSocket object.
 */
class TcpSocket: public QObject
{
    Q_OBJECT

public:
    TcpSocket(QTcpSocket* socket);

    QTcpSocket* getSocket() { return this->socket; }

signals:
    void sendDataReceived(QByteArray data, QHostAddress address, int port);
    void sendDisconnected(TcpSocket* socket);

private:
    QTcpSocket* socket;
    qint64 blockSize;

private slots:
    void on_readyRead();
    void on_disconnected();
};

#endif // TCPSOCKET_H
