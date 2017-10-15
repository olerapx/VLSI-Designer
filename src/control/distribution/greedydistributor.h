#pragma once

#include "distributor.h"

/**
 * @brief The DefaultDistributor class
 * Performs the greedy hierarchical distribution.
 */
class GreedyDistributor : public Distributor
{
public:
    GreedyDistributor(Client& client, FileSystem& system);

    void start(Scheme* scheme, int initialLevel) override;
    void stop() override;

    void onIncomingGrid(Grid* grid, int level, Statistics* statistics) override;
    void onReceivedNodes(int level) override;

private:
    void proceed(Scheme* scheme, int currentLevel);

    void handleLastLevel(Scheme* scheme, int currentLevel, Statistics* statistics);
    void distributeToNextLevel(Scheme* scheme, int currentLevel, Statistics* statistics);

    int initialLevel;
};
