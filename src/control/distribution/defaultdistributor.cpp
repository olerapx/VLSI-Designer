#include "defaultdistributor.h"

DefaultDistributor::DefaultDistributor(Client& client, QString currentSessionPath) :
    Distributor(client, currentSessionPath)
{

}

void DefaultDistributor::start(Scheme* scheme, int initialLevel)
{
    this->initialLevel = initialLevel;
    this->scheme = scheme;

    writeScheme(scheme, initialLevel);

    if(isLastLevel(initialLevel))
    {
        handleLastLevel(initialLevel);
        return;
    }
    distributeToNextLevel(initialLevel);
}

void DefaultDistributor::stop()
{
    client.sendStop();
    delete scheme;
}

void DefaultDistributor::handleLastLevel(int initialLevel)
{
   QObject* obj = new QObject(this);

   QObject::connect(&client, &Client::sendRoutedGrid, obj, [this, obj, initialLevel] (Grid* grid)
   {
       writeGridImage(grid, scheme, initialLevel);

       delete scheme;
       scheme = nullptr;

       writeGrid(grid, initialLevel);

       sendResult(grid, initialLevel);
       obj->deleteLater();
   });

   client.startPlacingAndRouting(scheme);
}

void DefaultDistributor::distributeToNextLevel(int initialLevel)
{
    writeScheme(scheme, initialLevel);

    int clientsNumber = getClientsNumberOnNextLevel(initialLevel);
    sendNeedNodes(clientsNumber);
}

void DefaultDistributor::onIncomingGrid(Grid* grid, int level)
{
    writeGridPart(grid, level);
    delete grid;

    if(getGridPartsNumber(level) != getClientsNumberOnNextLevel(level - 1))
        return;

    QList<Grid*> grids = readGridParts(level);
    Scheme* scheme = readScheme(level - 1);

    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendComposedGrid, obj, [this, obj, grids, scheme] (Grid* grid, int level)
    {
        writeGrid(grid, level);
        writeGridImage(grid, scheme, level);
        sendResult(grid, level);

        delete scheme;

        for(Grid* g: grids)
            delete g;

        obj->deleteLater();
    });

    client.startComposition(grids, scheme, level);
}

void DefaultDistributor::onError(QString)
{
    // TODO
}

void DefaultDistributor::onReceivedNodes()
{
    int clientsNumber = getClientsNumberOnNextLevel(initialLevel);

    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendDecomposedSchemes, obj, [this, obj] (QList<Scheme*> schemes)
    {
        for(Scheme* s: schemes)
        {
            writeSchemePart(s, initialLevel + 1);
            sendSchemePart(s, initialLevel + 1);

            delete scheme;
            scheme = nullptr;
        }

        obj->deleteLater();
    });

    client.startDecomposition(scheme, clientsNumber);
}
