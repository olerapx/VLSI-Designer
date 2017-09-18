#include "poolnodeinfo.h"

PoolNodeInfo::PoolNodeInfo(QString hostName, QHostAddress address, int tcpPort) :
    hostName(hostName),
    address(address),
    tcpPort(tcpPort)
{
    status = NodeStatus::Unconnected;
    programVersion = 0.0;
}
