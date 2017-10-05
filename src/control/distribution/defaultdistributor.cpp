#include "defaultdistributor.h"

DefaultDistributor::DefaultDistributor(Client& client, FileSystem& system) :
    Distributor(client, system)
{

}

void DefaultDistributor::start(Scheme* scheme, int initialLevel)
{
    writeScheme(scheme, initialLevel);

    if(isLastLevel(initialLevel))
    {
        handleLastLevel(scheme, initialLevel);
        return;
    }
    distributeToNextLevel(scheme, initialLevel);
}

void DefaultDistributor::stop()
{
    client.sendStop();
}

void DefaultDistributor::handleLastLevel(Scheme* scheme, int initialLevel)
{
   QObject* obj = new QObject(this);

   QObject::connect(&client, &Client::sendRoutedGrid, obj, [this, scheme, obj, initialLevel] (Grid* grid)
   {
       writeGridImage(grid, scheme, initialLevel);
       writeGrid(grid, initialLevel);

       delete scheme;

       sendResult(grid, initialLevel);
       obj->deleteLater();
   });

   client.startPlacingAndRouting(scheme);
}

void DefaultDistributor::distributeToNextLevel(Scheme* scheme, int initialLevel)
{
    writeScheme(scheme, initialLevel);

    int clientsNumber = getClientsNumberOnNextLevel(initialLevel);    

    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendDecomposedSchemes, obj, [this, obj, initialLevel, scheme, clientsNumber] (QList<Scheme*> schemes)
    {
        for(Scheme* s: schemes)
        {
            writeSchemePart(s, initialLevel + 1);
        }

        delete scheme;

        sendNeedNodes(initialLevel + 1, clientsNumber);

        obj->deleteLater();
    });

    client.startDecomposition(scheme, clientsNumber);
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
    stop();
    // TODO
}

void DefaultDistributor::onReceivedNodes(int level)
{
    QList<Scheme*> schemes = readSchemeParts(level);

    for(Scheme* s: schemes)
        sendSchemePart(s, level);
}
