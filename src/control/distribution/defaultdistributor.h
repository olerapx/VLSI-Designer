#pragma once

#include "distributor.h"

/**
 * @brief The DefaultDistributor class
 * Performs the default hierarchical distribution.
 */
class DefaultDistributor : public Distributor
{
public:
    DefaultDistributor(Client& client, QString currentSessionPath);

    void start(Scheme* scheme, int initialLevel) override;
    void stop() override;

    void onIncomingGrid(Grid* grid, int level) override;
    void onError(QString error) override;
    void onReceivedNodes() override;

private:
    void handleLastLevel(int initialLevel);
    void distributeToNextLevel(int initialLevel);
};
