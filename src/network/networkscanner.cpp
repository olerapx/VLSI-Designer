#include "networkscanner.h"

NetworkScanner::~NetworkScanner()
{
    if (mode != Mode::None)
        deleteSockets();
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
    if(!scanningDownstreamSocket->bind(QHostAddress::AnyIPv6, this->scanningPort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(tr("Cannot bind socket to port %1.").arg(scanningPort));

    if(!responseDownstreamSocket->bind(this->responsePort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(tr("Cannot bind socket to port %1.").arg(responsePort));

    scanningUpstreamSocket->setMulticastInterface(this->interface);
    scanningDownstreamSocket->joinMulticastGroup(this->scanningAddress, this->interface);

    scanningUpstreamSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));
    scanningDownstreamSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));

    mode = Mode::IPv6;
}

void NetworkScanner::initIPv4Broadcast(QNetworkInterface interface, int scanningPort, int responsePort)
{
    stopScanning();

    this->interface = interface;
    this->scanningPort = scanningPort;
    this->responsePort = responsePort;

    initScanningSockets();

    scanningAddress = findAnyBroadcastAddress();
    if(scanningAddress == QHostAddress::Null) throw NetworkException(tr("No broadcast address found for interface: %1.")
                                                                      .arg(interface.humanReadableName()));

    scanningUpstreamSocket->bind();

    if(!scanningDownstreamSocket->bind(QHostAddress::AnyIPv4, this->scanningPort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(tr("Cannot bind socket to port %1.").arg(scanningPort));

    if(!responseDownstreamSocket->bind(this->responsePort,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(tr("Cannot bind socket to port %1.").arg(responsePort));

    mode = Mode::IPv4;
}

QHostAddress NetworkScanner::findAnyBroadcastAddress()
{
    QList<QNetworkAddressEntry> nets = interface.addressEntries();

    for(QNetworkAddressEntry entry: nets)
    {
        if(!entry.broadcast().isNull())
            return entry.broadcast();
    }

    return QHostAddress::Null;
}

void NetworkScanner::initScanningSockets()
{
    if(mode != Mode::None)
        deleteSockets();
    mode = Mode::None;

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

        sendLog(tr("[Client] Got request: %1.").arg(QString::fromUtf8(datagram)));
        sendLog(tr("[Client] Sending response to %1.").arg(senderHost.toString()));

        datagram.append("@"+QHostInfo::localHostName());

        QUdpSocket* udpSocket = new QUdpSocket(this);

        udpSocket->writeDatagram(datagram.data(), datagram.size(), senderHost, responsePort);

        udpSocket->close();
        delete udpSocket;
    }
}

void NetworkScanner::processResponseDatagrams()
{
    while(responseDownstreamSocket->hasPendingDatagrams() && !stopped)
    {
        QByteArray datagram;
        datagram.resize(responseDownstreamSocket->pendingDatagramSize());

        QHostAddress senderHost;

        responseDownstreamSocket->readDatagram(datagram.data(), datagram.size(), &senderHost);

        sendLog(tr("[Server] Got response from %1:%2.").arg(senderHost.toString(), QString::fromUtf8(datagram)));

        QString datagramString = QString(datagram);

        QString token = datagramString.section("@", 0, 0);

        if(token == currentScanToken.toUtf8())
        {
            QString hostName = datagramString.section("@", 1);

            sendLog(tr("[Server] Auth confirmed: %1.").arg(hostName));
            sendAddress(senderHost, hostName);
        }
        else sendLog(tr("[Server] Wrong auth token, ignored."));
    }
}

void NetworkScanner::scanNetwork()
{
    if(mode == Mode::None)
        throw NetworkException(tr("Scanner is not initialized."));

    if(!stopped)
        throw NetworkException(tr("Already scanning."));

    stopped = false;

    currentScanToken = QUuid::createUuid().toString();

    sendLog(tr("[Server] Sending auth request with token: %1.").arg(currentScanToken));

    QByteArray data = currentScanToken.toUtf8();
    scanningUpstreamSocket->writeDatagram(data.data(), data.size(), scanningAddress, scanningPort);
}

void NetworkScanner::stopScanning()
{
    stopped = true;
}
