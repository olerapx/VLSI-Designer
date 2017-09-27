#include "configbuilder.h"

Config ConfigBuilder::readConfig()
{
    return createDefault(); // TODO
}

Config ConfigBuilder::createDefault()
{
    Config config;

    config.setMode(Mode::IPv6);
    config.setMulticastAddress(QHostAddress("FF02::1"));

    config.setUdpPort(40000);
    config.setTcpPorts(40000, 40001);

    config.setLibrariesPath("libraries");
    config.setSessionsPath("sessions");

    return config;
}
