#include "config.h"

Config::Config()
{
    setMode(Mode::IPv6);
    setMulticastAddress(QHostAddress("FF02::1"));

    setUdpPort(40000);
    setTcpPort(40001);

    networkInterface = nullptr;
}

void Config::setMode(Mode mode)
{
    this->mode = mode;
}

void Config::setMulticastAddress(QHostAddress multicastAddress)
{
    this->multicastAddress = multicastAddress;
}

void Config::setNetworkInterface(QNetworkInterface interface)
{
    delete networkInterface;
    networkInterface = new QNetworkInterface(interface);
}

void Config::setUdpPort(int udpPort)
{
    this->udpPort = udpPort;
}

void Config::setTcpPort(int port)
{
    this->tcpPort = port;
}
