#include "poolnodeinfo.h"

PoolNodeInfo::PoolNodeInfo(QString hostName, QHostAddress address, int tcpPort) :
    hostName(hostName),
    address(address),
    tcpPort(tcpPort)
{
    status = NodeStatus::Unconnected;
    programVersion = 0.0;
}

void PoolNodeInfo::setStatus(NodeStatus status)
{
    this->status = status;
}

void PoolNodeInfo::setProgramVersion(double version)
{
    this->programVersion = version;
}

bool PoolNodeInfo::operator ==(const PoolNodeInfo &other) const
{
    if(address == other.getAddress() && tcpPort == other.getTcpPort())
        return true;

    return false;
}
