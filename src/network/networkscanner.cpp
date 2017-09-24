#include "networkscanner.h"

NetworkScanner::NetworkScanner(int& nodeTcpPort) :
    nodeTcpPort(nodeTcpPort)
{

}

NetworkScanner::~NetworkScanner()
{
    if (mode != Mode::None)
        deleteSockets();
}

void NetworkScanner::initIPv6Multicast(QHostAddress scanningAddress, QNetworkInterface interface, int port)
{
    stopScanning();

    this->scanningAddress = scanningAddress;
    this->interface = interface;
    this->port = port;

    initScanningSockets();

    upstreamSocket->bind();
    if(!downstreamSocket->bind(QHostAddress::AnyIPv6, this->port,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(tr("Cannot bind socket to port %1.").arg(port));

    upstreamSocket->setMulticastInterface(this->interface);
    downstreamSocket->joinMulticastGroup(this->scanningAddress, this->interface);

    upstreamSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));
    downstreamSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));

    mode = Mode::IPv6;
}

void NetworkScanner::initIPv4Broadcast(QNetworkInterface interface, int port)
{
    stopScanning();

    this->interface = interface;
    this->port = port;

    initScanningSockets();

    scanningAddress = findAnyBroadcastAddress();
    if(scanningAddress == QHostAddress::Null) throw NetworkException(tr("No broadcast address found for interface: %1.")
                                                                      .arg(interface.humanReadableName()));

    upstreamSocket->bind();

    if(!downstreamSocket->bind(QHostAddress::AnyIPv4, this->port,
                                        QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress))
        throw NetworkException(tr("Cannot bind socket to port %1.").arg(port));

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

    upstreamSocket = new QUdpSocket(this);
    downstreamSocket = new QUdpSocket(this);

    connect(downstreamSocket, &QUdpSocket::readyRead, this, &NetworkScanner::processDatagrams);
}

void NetworkScanner::deleteSockets()
{
    upstreamSocket->close();
    downstreamSocket->close();

    delete upstreamSocket;
    delete downstreamSocket;
}

void NetworkScanner::processDatagrams()
{
    while(downstreamSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(downstreamSocket->pendingDatagramSize());

        QHostAddress senderHost;

        downstreamSocket->readDatagram(datagram.data(), datagram.size(), &senderHost);

        handleDatagram(datagram, senderHost);
    }
}

void NetworkScanner::handleDatagram(QByteArray datagram, QHostAddress senderHost)
{
    QDataStream stream(&datagram, QIODevice::ReadOnly);
    qint32 type = 0;
    stream >> type;

    DatagramType datagramType = static_cast<DatagramType>(type);

    datagram.remove(0, sizeof(qint32));

    if(datagramType == DatagramType::Scan)
        processScanningDatagram(datagram, senderHost);
    else
        processResponseDatagram(datagram, senderHost);
}

void NetworkScanner::processScanningDatagram(QByteArray datagram, QHostAddress senderHost)
{
    sendLog(tr("[Client] Got request: %1.").arg(QString::fromUtf8(datagram)));
    sendLog(tr("[Client] Sending response to %1.").arg(senderHost.toString()));

    datagram.append("@" + QHostInfo::localHostName());
    datagram.append("@" + QString::number(nodeTcpPort));
    prependDatagramType(datagram, DatagramType::Response);

    QUdpSocket* udpSocket = new QUdpSocket(this);

    udpSocket->writeDatagram(datagram.data(), datagram.size(), senderHost, port);

    udpSocket->close();
    delete udpSocket;
}

void NetworkScanner::prependDatagramType(QByteArray& datagram, DatagramType type)
{
    datagram.prepend(sizeof(qint32), ' ');

    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream.device()->seek(0);
    stream << static_cast<qint32>(type);
}

void NetworkScanner::processResponseDatagram(QByteArray datagram, QHostAddress senderHost)
{
    sendLog(tr("[Server] Got response from %1:%2.").arg(senderHost.toString(), QString::fromUtf8(datagram)));

    QString datagramString = QString(datagram);

    QUuid token(datagramString.section("@", 0, 0));

    if(token == currentScanToken)
    {
        QString hostName = datagramString.section("@", 1, 1);
        int hostTcpPort = datagramString.section("@", 2, 2).toInt();

        sendLog(tr("[Server] Auth confirmed: %1.").arg(hostName));
        sendAddress(senderHost, hostName, hostTcpPort);
    }
    else sendLog(tr("[Server] Wrong auth token, ignored."));
}

void NetworkScanner::scanNetwork()
{
    if(mode == Mode::None)
        throw NetworkException(tr("Scanner is not initialized."));

    if(!stopped)
        throw NetworkException(tr("Already scanning."));

    stopped = false;

    currentScanToken = QUuid::createUuid();

    sendLog(tr("[Server] Sending auth request with token: %1.").arg(currentScanToken.toString()));

    QByteArray data = currentScanToken.toString().toUtf8();
    prependDatagramType(data, DatagramType::Scan);

    upstreamSocket->writeDatagram(data.data(), data.size(), scanningAddress, port);
}

void NetworkScanner::stopScanning()
{
    stopped = true;
}
