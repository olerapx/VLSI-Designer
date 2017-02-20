#include "networkscanner.h"

NetworkScanner::NetworkScanner()
{

}

NetworkScanner::~NetworkScanner()
{
    if (mode != None) deleteSockets();
}

void NetworkScanner::initIPv6Multicast(QHostAddress scanningAddress, QNetworkInterface interface,
                                       int scanningPort, int responsePort)
{
    stopScanning();

    this->scanningAddress = scanningAddress;
    this->interface = interface;
    this->scanningPort = scanningPort;
    this->responsePort = responsePort;

    initScanningSockets();

    scanningUpstreamSocket->bind();
    if (!scanningDownstreamSocket->bind(QHostAddress::AnyIPv6, this->scanningPort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(QString("Cannot bind socket to port %1.").arg(scanningPort));

    if (!responseDownstreamSocket->bind(this->responsePort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(QString("Cannot bind socket to port %1.").arg(responsePort));

    scanningUpstreamSocket->setMulticastInterface(this->interface);
    scanningDownstreamSocket->joinMulticastGroup(this->scanningAddress, this->interface);

    scanningUpstreamSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));
    scanningDownstreamSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));

    mode = IPv6;
}

void NetworkScanner::initIPv4Broadcast(QNetworkInterface interface, int scanningPort, int responsePort)
{
    stopScanning();

    this->interface = interface;
    this->scanningPort = scanningPort;
    this->responsePort = responsePort;

    initScanningSockets();

    scanningAddress = findAnyBroadcastAddress();
    if (scanningAddress == QHostAddress::Null) throw NetworkException ("No broadcast address found.");

    scanningUpstreamSocket->bind();

    if (!scanningDownstreamSocket->bind(QHostAddress::AnyIPv4, this->scanningPort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(QString("Cannot bind socket to port %1.").arg(scanningPort));

    if (!responseDownstreamSocket->bind(this->responsePort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(QString("Cannot bind socket to port %1.").arg(responsePort));

    mode = IPv4;
}

QHostAddress NetworkScanner::findAnyBroadcastAddress()
{
    QList<QNetworkAddressEntry> nets = interface.addressEntries();

    foreach(QNetworkAddressEntry entry, nets)
    {
        if (!entry.broadcast().isNull())
            return entry.broadcast();
    }

    return QHostAddress::Null;
}

void NetworkScanner::initScanningSockets()
{
    if (mode != None) deleteSockets();
    mode = None;

    scanningUpstreamSocket = new QUdpSocket(this);
    scanningDownstreamSocket = new QUdpSocket(this);
    responseDownstreamSocket = new QUdpSocket(this);

    connect(scanningDownstreamSocket, &QUdpSocket::readyRead, this, &NetworkScanner::processScanningDatagrams);
    connect(responseDownstreamSocket, &QUdpSocket::readyRead, this, &NetworkScanner::processResponseDatagrams);
}

void NetworkScanner::deleteSockets()
{
    scanningUpstreamSocket->close();
    scanningDownstreamSocket->close();
    responseDownstreamSocket->close();

    disconnect(scanningDownstreamSocket, &QUdpSocket::readyRead, this, &NetworkScanner::processScanningDatagrams);
    disconnect(responseDownstreamSocket, &QUdpSocket::readyRead, this, &NetworkScanner::processResponseDatagrams);

    delete scanningUpstreamSocket;
    delete scanningDownstreamSocket;
    delete responseDownstreamSocket;
}

void NetworkScanner::processScanningDatagrams()
{
    while (scanningDownstreamSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(scanningDownstreamSocket->pendingDatagramSize());

        QHostAddress senderHost;

        scanningDownstreamSocket->readDatagram(datagram.data(), datagram.size(), &senderHost);

        sendLog(QString("[Client] Got request: ") + datagram);
        sendLog("[Client] Sending response to " + senderHost.toString());

        datagram.append("@"+QHostInfo::localHostName());

        QUdpSocket* udpSocket = new QUdpSocket(this);

        udpSocket->writeDatagram(datagram.data(), datagram.size(), senderHost, responsePort);

        udpSocket->close();
        delete udpSocket;
    }
}

void NetworkScanner::processResponseDatagrams()
{
    while (responseDownstreamSocket->hasPendingDatagrams() && !stopped)
    {
        QByteArray datagram;
        datagram.resize(responseDownstreamSocket->pendingDatagramSize());

        QHostAddress senderHost;

        responseDownstreamSocket->readDatagram(datagram.data(), datagram.size(), &senderHost);

        sendLog("[Server] Got response from " + senderHost.toString() + ":" + datagram);

        QString datagramString = QString(datagram);

        QString token = datagramString.section("@", 0, 0);

        if (token == currentScanToken.toUtf8())
        {
            QString hostName = datagramString.section("@", 1);

            sendLog ("[Server] Auth confirmed.");
            sendAddress(senderHost, hostName);
        }
        else sendLog ("[Server] Wrong auth token, ignored.");
    }
}

void NetworkScanner::scanNetwork()
{
    if (mode == None)
        throw NetworkException ("Scanner is not initialized");

    if (!stopped)
        throw NetworkException ("Already scanning.");

    stopped = false;

    currentScanToken = QUuid::createUuid().toString();

    sendLog("[Server] Sending auth request with token: " + currentScanToken);

    QByteArray data = currentScanToken.toUtf8();
    scanningUpstreamSocket->writeDatagram(data.data(), data.size(), scanningAddress, scanningPort);
}

void NetworkScanner::stopScanning()
{
    stopped = true;
}
