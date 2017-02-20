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

/**
 * @brief Scans for hosts in a given network using IPv4 broadcast or IPv6 multicast requests.
 *
 * This class represents a UDP listener and transciever and is used to obtain all active hosts in a given network.
 * All hosts' scanners should be initialized identically to the server's scanner.
 * In order to use the scanner, you should call initIPv6Multicast() or initIPv4Broadcast() to initialize it.
 * After that, you can call scanNetwork() to start scanning. The scanner will send an authorization request with
 * a randomly generated UUID to a chosen network and will be awaiting for responses until stopScanning() is called.
 * Each initialized scanner will constantly listen to a given scanning port. When an auth request is read, it will
 * send a response to a specified response port.
 * Each response will be classified as a new host by the server.
 */
class NetworkScanner: public QObject
{
    Q_OBJECT

public:
    NetworkScanner();
    ~NetworkScanner();

    void initIPv6Multicast(QHostAddress scanningAddress, QNetworkInterface interface, int scanningPort, int responsePort);
    void initIPv4Broadcast(QNetworkInterface interface, int scanningPort, int responsePort);

    void scanNetwork();
    void stopScanning();

    bool isStopped() { return this->stopped; }

signals:
    void sendLog (QString data);

    /**
     * @brief Emits when a new host is discovered.
     * @param senderHost - an address and port.
     * @param hostName - a human readable name.
     */
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
