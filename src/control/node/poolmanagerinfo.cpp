#include "poolmanagerinfo.h"

PoolManagerInfo::PoolManagerInfo(QString hostName, QHostAddress address, int tcpPort) :
    hostName(hostName),
    address(address),
    tcpPort(tcpPort)
{

}
