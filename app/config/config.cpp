#include "config.h"

Config::Config()
{
    setMode(Mode::IPv6);
    setMulticastAddress(QHostAddress("FF02::1"));

    setUdpPort(40000);
    setTcpPorts(40000, 40001);
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

void Config::setTcpPorts(int managerTcpPort, int nodeTcpPort)
{
    validatePort(managerTcpPort);
    validatePort(nodeTcpPort);

    if(managerTcpPort == nodeTcpPort)
        throw IllegalArgumentException(QObject::tr("Manager and node tcp ports cannot be equal."));

    this->managerTcpPort = managerTcpPort;
    this->nodeTcpPort = nodeTcpPort;
}

void Config::validatePort(int port)
{
    if(port < 0 || port > 65535)
        throw IllegalArgumentException(QObject::tr("Port is out of range, passed: %1.").arg(QString::number(port)));
}
