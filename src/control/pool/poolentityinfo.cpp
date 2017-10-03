#include "poolentityinfo.h"

PoolEntityInfo::PoolEntityInfo(QString hostName, QHostAddress address, int tcpPort) :
    hostName(hostName),
    address(address),
    tcpPort(tcpPort)
{
    status = EntityStatus::Unconnected;
}

void PoolEntityInfo::setStatus(EntityStatus status)
{
    this->status = status;
}

void PoolEntityInfo::setProgramVersion(Version version)
{
    this->programVersion = version;
}

bool PoolEntityInfo::operator ==(const PoolEntityInfo &other) const
{
    if(address == other.getAddress() && tcpPort == other.getTcpPort())
        return true;

    return false;
}
