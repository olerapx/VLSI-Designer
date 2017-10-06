#include "defaultdistributor.h"

DefaultDistributor::DefaultDistributor(Client& client, FileSystem& system) :
    Distributor(client, system)
{

}

void DefaultDistributor::start(Scheme* scheme, int initialLevel)
{
    writeScheme(scheme, initialLevel);

    Statistics* statistics = new Statistics(initialLevel);
    statistics->getData().append(QList<StatisticsEntry>());
    statistics->getData()[0].append(StatisticsEntry());
    statistics->getData()[0][0].setHostName(QHostInfo::localHostName());

    if(isLastLevel(initialLevel))
    {
        handleLastLevel(scheme, initialLevel, statistics);
        return;
    }
    distributeToNextLevel(scheme, initialLevel, statistics);
}

void DefaultDistributor::stop()
{
    client.stop();
}

void DefaultDistributor::handleLastLevel(Scheme* scheme, int initialLevel, Statistics* statistics)
{
    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendStop, obj, [this, scheme, obj] ()
    {
        delete scheme;
        obj->deleteLater();
    });

    QObject::connect(&client, &Client::sendRoutedGrid, obj, [this, scheme, obj, initialLevel, statistics] (Grid* grid)
    {
        writeGridImage(grid, scheme, initialLevel);
        writeGrid(grid, initialLevel);
        writeStatistics(statistics, initialLevel);

        delete scheme;

        sendResult(grid, initialLevel, statistics);
        obj->deleteLater();
    });

    client.startPlacingAndRouting(scheme, statistics->getData()[0][0]);
}

void DefaultDistributor::distributeToNextLevel(Scheme* scheme, int initialLevel, Statistics* statistics)
{
    writeScheme(scheme, initialLevel);

    int clientsNumber = getClientsNumberOnNextLevel(initialLevel);

    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendStop, obj, [this, scheme, obj] ()
    {
        delete scheme;
        obj->deleteLater();
    });

    QObject::connect(&client, &Client::sendDecomposedSchemes, obj, [this, obj, initialLevel, scheme, clientsNumber, statistics] (QList<Scheme*> schemes)
    {
        for(Scheme* s: schemes)
        {
            writeSchemePart(s, initialLevel + 1);
        }

        delete scheme;

        writeStatistics(statistics, initialLevel);
        delete statistics;

        sendNeedNodes(initialLevel + 1, clientsNumber);

        obj->deleteLater();
    });

    client.startDecomposition(scheme, clientsNumber, statistics->getData()[0][0]);
}

void DefaultDistributor::onIncomingGrid(Grid* grid, int level, Statistics* statistics)
{
    writeGridPart(grid, level);
    delete grid;

    Statistics* commonStatistics = readStatistics(level - 1);
    commonStatistics->merge(*statistics);
    delete statistics;

    writeStatistics(commonStatistics, level - 1);
    delete commonStatistics;

    if(getGridPartsNumber(level) != getClientsNumberOnNextLevel(level - 1))
        return;

    QList<Grid*> grids = readGridParts(level);
    Scheme* scheme = readScheme(level - 1);
    commonStatistics = readStatistics(level - 1);

    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendStop, obj, [this, obj, grids, scheme] ()
    {
        delete scheme;

        for(Grid* g: grids)
            delete g;

        obj->deleteLater();
    });

    QObject::connect(&client, &Client::sendComposedGrid, obj, [this, obj, grids, scheme, commonStatistics] (Grid* grid, int level)
    {
        writeGrid(grid, level);
        writeGridImage(grid, scheme, level);
        sendResult(grid, level, commonStatistics);

        delete scheme;

        for(Grid* g: grids)
            delete g;

        obj->deleteLater();
    });

    client.startComposition(grids, scheme, level, commonStatistics->getData()[0][0]);
}

void DefaultDistributor::onReceivedNodes(int level)
{
    QList<Scheme*> schemes = readSchemeParts(level);

    for(Scheme* s: schemes)
        sendSchemePart(s, level);
}
