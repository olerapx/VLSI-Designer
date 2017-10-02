#pragma once

#include "control/pool/node/client.h"

/**
 * @brief The Distributor class
 * Base class for distribution strategies.
 */
class Distributor : public QObject
{
    Q_OBJECT

public:
    Distributor(Client& client);

    /**
     * @brief start
     * Starts the design process or continues it depends on initialLevel.
     * @param scheme
     * @param initialLevel
     */
    virtual void start(Scheme* scheme, int initialLevel) = 0;

signals:
    /**
     * @brief sendResult
     * Emits when all the necessary work is done and the grid should be sent to the sender host.
     * @param grid
     * @param level
     */
    void sendResult(Grid* grid, int level);

    /**
     * @brief sendSchemePart
     * Emits when a scheme part should be transmitted to another node.
     * @param scheme
     * @param level
     */
    void sendSchemePart(Scheme* scheme, int level);

    /**
     * @brief sendNeedNodes
     * Emits when the nodes are needed to continue the design process.
     * @param number
     */
    void sendNeedNodes(int number);

public slots:    
    virtual void onIncomingGrid(Grid* grid, int level) = 0;    
    virtual void onError(QString error) = 0;
    virtual void onReceivedNodes() = 0;

protected:
    int getLevelsNumber() const;
    int getClientsNumberOnNextLevel(int levelNumber) const;
    bool isLastLevel(int levelNumber) const;

    Client& client;

    int initialLevel;
    int currentLevel;
};
