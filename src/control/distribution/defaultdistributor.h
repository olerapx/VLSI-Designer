#pragma once

#include "distributor.h"

/**
 * @brief The DefaultDistributor class
 * Performs the default hierarchical distribution.
 */
class DefaultDistributor : public Distributor
{
public:
    DefaultDistributor(Client& client, FileSystem& system);

    void start(Scheme* scheme, int initialLevel) override;
    void stop() override;

    void onIncomingGrid(Grid* grid, int level, Statistics* statistics) override;
    void onReceivedNodes(int level) override;

private:
    void handleLastLevel(Scheme* scheme, int initialLevel, Statistics* statistics);
    void distributeToNextLevel(Scheme* scheme, int initialLevel, Statistics* statistics);
};
