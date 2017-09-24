#include "config.h"

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

QString Config::getLibrariesPath()
{
    QDir dir = QDir::currentPath();

    #ifdef QT_DEBUG
        dir.cdUp();
    #endif

    return dir.absolutePath() + "/" + librariesPath;
}

void Config::setLibrariesPath(QString path)
{
    this->librariesPath = path;
}
