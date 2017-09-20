#pragma once

#include <QUdpSocket>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QUuid>
#include <QHostInfo>
#include <QDataStream>

#include "networkexception.h"

enum class Mode
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
class NetworkScanner : public QObject
{
    Q_OBJECT

public:
    NetworkScanner(int& nodeTcpPort);
    ~NetworkScanner();

    /**
     * @brief initIPv6Multicast
     * Initializes NetworkScanner to use IPv6 multicast requests.
     * @param scanningAddress - the IPv6 multicast address
     * @param interface
     * @param port
     * @throw NetworkException
     */
    void initIPv6Multicast(QHostAddress scanningAddress, QNetworkInterface interface, int port);

    /**
     * @brief initIPv4Broadcast
     * Initializes the scanner to use IPv4 broadcast requests.
     * @param interface
     * @param port
     */
    void initIPv4Broadcast(QNetworkInterface interface, int port);

    /**
     * @brief scanNetwork
     * Starts stanning the network.
     * The scanner must be initialized and not active before calling.
     * @throw NetworkException
     */
    void scanNetwork();

    /**
     * @brief stopScanning
     * Stops scanning the network.
     */
    void stopScanning();

    bool isStopped() const { return this->stopped; }

signals:
    void sendLog (QString data);

    /**
     * @brief Emits when a new host is discovered.
     * @param senderHost - the address and port.
     * @param hostName - the human readable name.
     */
    void sendAddress(QHostAddress senderHost, QString hostName, int nodeTcpPort);

private:
    enum class DatagramType
    {
        Scan,
        Response
    };

    int& nodeTcpPort;

    QUdpSocket* upstreamSocket;
    QUdpSocket* downstreamSocket;

    QNetworkInterface interface;

    QHostAddress scanningAddress;
    int port;

    bool stopped = true;
    Mode mode = Mode::None;

    QString currentScanToken;

    void initScanningSockets();
    QHostAddress findAnyBroadcastAddress();
    void deleteSockets();

    void handleDatagram(QByteArray datagram, QHostAddress senderHost);
    void processScanningDatagram(QByteArray datagram, QHostAddress senderHost);
    void processResponseDatagram(QByteArray datagram, QHostAddress senderHost);

    void prependDatagramType(QByteArray& datagram, DatagramType type);

private slots:
    void processDatagrams();
};
