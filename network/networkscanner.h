#ifndef NETWORKSCANNER_H
#define NETWORKSCANNER_H

#include <QUdpSocket>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QUuid>
#include <QHostInfo>

#include "networkexception.h"

enum Mode
{
    IPv4,
    IPv6,
    None
};

class NetworkScanner: public QObject
{
    Q_OBJECT

public:
    NetworkScanner();

    void initIPv6Multicast(QHostAddress scanningAddress, QNetworkInterface interface, int scanningPort, int responsePort);
    void initIPv4Broadcast(QNetworkInterface interface, int scanningPort, int responsePort);

    void scanNetwork();
    void stopScanning();

    bool isStopped() { return this->stopped; }

signals:
    void sendLog (QString data);
    void sendAddress(QHostAddress senderHost, QString hostName);

private:
    QUdpSocket* scanningUpstreamSocket;
    QUdpSocket* scanningDownstreamSocket;
    QUdpSocket* responseDownstreamSocket;

    QNetworkInterface interface;

    QHostAddress scanningAddress;
    int scanningPort;
    int responsePort;

    bool stopped = true;
    Mode mode = None;

    QString currentScanToken;

    void initScanningSockets();
    QHostAddress findAnyBroadcastAddress();
    void deleteSockets();

private slots:
    void processScanningDatagrams();
    void processResponseDatagrams();
};

#endif // NETWORKSCANNER_H
