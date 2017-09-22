#pragma once

#include "config.h"

/**
 * @brief The ConfigBuilder class
 * Provides reading/writing config object.
 */
class ConfigBuilder // TODO: write config
{
public:
    /**
     * @brief readConfig
     * Creates a config object from config.json.
     * @return
     */
    static Config readConfig();

    /**
     * @brief createDefault
     * Creates a default config object.
     * @return
     */
    static Config createDefault();
};
