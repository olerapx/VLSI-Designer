#include "poolnodeinfo.h"

PoolNodeInfo::PoolNodeInfo(QString hostName, QHostAddress address, int tcpPort) :
    hostName(hostName),
    address(address),
    tcpPort(tcpPort)
{
    status = NodeStatus::Unconnected;
}

void PoolNodeInfo::setStatus(NodeStatus status)
{
    this->status = status;
}

void PoolNodeInfo::setProgramVersion(Version version)
{
    this->programVersion = version;
}

bool PoolNodeInfo::operator ==(const PoolNodeInfo &other) const
{
    if(address == other.getAddress() && tcpPort == other.getTcpPort())
        return true;

    return false;
}
