#include "poolnodeinfo.h"

PoolNodeInfo::PoolNodeInfo(QString hostName, QHostAddress address) :
    hostName(hostName),
    address(address)
{
    status = NodeStatus::Unconnected;
    programVersion = 0.0;
}
