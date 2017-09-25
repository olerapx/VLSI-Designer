#pragma once

#include "datamodels/version/version.h"

/**
 * @brief The App class
 * Contains the common application info.
 */
class App
{
public:
    static const Version APP_VERSION;

private:
    App() {}
};
