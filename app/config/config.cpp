#include "config.h"

Config::Config()
{
    mode = Mode::None;
    port = 0;
}

void Config::setMode(Mode mode)
{
    this->mode = mode;
}

void Config::setMulticastAddress(QHostAddress multicastAddress)
{
    this->multicastAddress = multicastAddress;
}

void Config::setPort(int port)
{
    this->port = port;
}
