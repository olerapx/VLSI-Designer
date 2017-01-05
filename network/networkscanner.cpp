#include "networkscanner.h"
#include <iostream>

NetworkScanner::NetworkScanner()
{

}

void NetworkScanner::initIPv6Multicast(QHostAddress scanningAddress, QNetworkInterface interface, int scanningPort, int responsePort)
{
    stopScanning();

    this->scanningAddress = scanningAddress;
    this->interface = interface;
    this->scanningPort = scanningPort;
    this->responsePort = responsePort;

    initScanningSockets();

    scanningUpstreamSocket->bind();
    scanningDownstreamSocket->bind(QHostAddress::AnyIPv6, this->scanningPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    responseDownstreamSocket->bind(this->responsePort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);

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
    scanningDownstreamSocket->bind(QHostAddress::AnyIPv4, this->scanningPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    responseDownstreamSocket->bind(this->responsePort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);

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

    connect(scanningDownstreamSocket, SIGNAL(readyRead()), this, SLOT(processScanningDatagrams()));
    connect(responseDownstreamSocket, SIGNAL(readyRead()), this, SLOT(processResponseDatagrams()));
}

void NetworkScanner::deleteSockets()
{
    scanningUpstreamSocket->close();
    scanningDownstreamSocket->close();
    responseDownstreamSocket->close();

    disconnect(scanningDownstreamSocket, SIGNAL(readyRead()), this, SLOT(processScanningDatagrams()));
    disconnect(responseDownstreamSocket, SIGNAL(readyRead()), this, SLOT(processResponseDatagrams()));

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

        QUdpSocket* udpSocket = new QUdpSocket(this);

        QByteArray responseData = "AUTH CONFIRMATION";
        udpSocket->writeDatagram(responseData.data(), responseData.size(), senderHost, responsePort);

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

        sendLog("[Server] Got response from " + senderHost.toString());
        sendAddress(senderHost);
    }
}

void NetworkScanner::scanNetwork()
{
    if (mode == None)
        throw NetworkException ("Scanner is not initialized");

    if (!stopped)
        throw NetworkException ("Already scanning.");

    stopped = false;

    sendLog("[Server] Sending auth request...");

    QByteArray data = "AUTH REQUEST";
    scanningUpstreamSocket->writeDatagram(data.data(), data.size(), scanningAddress, scanningPort);
}

void NetworkScanner::stopScanning()
{
    stopped = true;
}
