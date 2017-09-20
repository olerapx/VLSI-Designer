#include "poolentity.h"

PoolEntity::PoolEntity(int port) :
    transmitter(nullptr),
    selfPort(port)
{

}

PoolEntity::~PoolEntity()
{
    disableTransmitter();
}

void PoolEntity::setSelfPort(int port)
{
    if(isTransmitterEnabled())
    {
        throw Exception(tr("Cannot set the port while the transmitter is enabled."));
    }
    this->selfPort = port;
}

void PoolEntity::enableTransmitter()
{
    if(transmitter != nullptr)
        return;

    transmitter = new NetworkTransmitter(selfPort);
}

void PoolEntity::disableTransmitter()
{
    if(transmitter == nullptr)
        return;

    delete transmitter;
    transmitter = nullptr;
}
