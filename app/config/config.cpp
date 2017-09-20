#include "config.h"

Config::Config()
{
    setMode(Mode::IPv6);
    setMulticastAddress(QHostAddress("FF02::1"));

    setUdpPort(40000);
    setManagerTcpPort(40000);
    setNodeTcpPort(40001);
}

void Config::setMode(Mode mode)
{
    this->mode = mode;
}

void Config::setMulticastAddress(QHostAddress multicastAddress)
{
    if(multicastAddress == QHostAddress::Null)
        throw IllegalArgumentException(QObject::tr("Multicast address is invalid."));

    this->multicastAddress = multicastAddress;
}

void Config::setNetworkInterface(QNetworkInterface interface)
{
    networkInterface = interface;
}

void Config::setUdpPort(int udpPort)
{
    validatePort(udpPort);
    this->udpPort = udpPort;
}

void Config::setManagerTcpPort(int tcpPort)
{
    validatePort(tcpPort);

    if(nodeTcpPort == tcpPort)
        throw IllegalArgumentException(QObject::tr("Manager and node tcp ports cannot be equal."));

    this->managerTcpPort = tcpPort;
}

void Config::setNodeTcpPort(int tcpPort)
{
    validatePort(tcpPort);

    if(managerTcpPort == tcpPort)
        throw IllegalArgumentException(QObject::tr("Manager and node tcp ports cannot be equal."));

    this->nodeTcpPort = tcpPort;
}

void Config::validatePort(int port)
{
    if(port < 0 || port > 65535)
        throw IllegalArgumentException(QObject::tr("Port is out of range, passed: %1.").arg(QString::number(port)));
}
