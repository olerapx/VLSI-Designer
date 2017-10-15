#include "greedydistributor.h"

GreedyDistributor::GreedyDistributor(Client& client, FileSystem& system) :
    Distributor(client, system),
    initialLevel(0)
{

}

void GreedyDistributor::start(Scheme* scheme, int initialLevel)
{
    this->initialLevel = initialLevel;
    proceed(scheme, initialLevel);
}

void GreedyDistributor::proceed(Scheme* scheme, int currentLevel)
{
    Statistics* statistics = new Statistics(currentLevel);
    statistics->getData().append(QList<StatisticsEntry>());
    statistics->getData()[0].append(StatisticsEntry());
    statistics->getData()[0][0].setHostName(QHostInfo::localHostName());

    if(isLastLevel(currentLevel))
    {
        handleLastLevel(scheme, currentLevel, statistics);
        return;
    }
    distributeToNextLevel(scheme, currentLevel, statistics);
}

void GreedyDistributor::stop()
{
    client.stop();
}

void GreedyDistributor::handleLastLevel(Scheme* scheme, int currentLevel, Statistics* statistics)
{
    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendStop, obj, [this, scheme, obj] ()
    {
        delete scheme;
        obj->deleteLater();
    });

    QObject::connect(&client, &Client::sendRoutedGrid, obj, [this, scheme, obj, currentLevel, statistics] (Grid* grid)
    {
        writeGridImage(grid, scheme, currentLevel);
        writeGrid(grid, currentLevel);
        writeStatistics(statistics, currentLevel);

        delete scheme;

        if(currentLevel == initialLevel)
            sendResult(grid, currentLevel, statistics);
        else
            onIncomingGrid(grid, currentLevel, statistics);

        obj->deleteLater();
    }, Qt::QueuedConnection);

    client.startPlacingAndRouting(scheme, statistics->getData()[0][0]);
}

void GreedyDistributor::distributeToNextLevel(Scheme* scheme, int currentLevel, Statistics* statistics)
{
    writeScheme(scheme, currentLevel);

    int clientsNumber = getClientsNumberOnNextLevel(currentLevel);

    QObject* obj = new QObject(this);

    QObject::connect(&client, &Client::sendStop, obj, [this, scheme, obj] ()
    {
        delete scheme;
        obj->deleteLater();
    });

    QObject::connect(&client, &Client::sendDecomposedSchemes, obj, [this, obj, currentLevel, scheme, clientsNumber, statistics] (QList<Scheme*> schemes)
    {
        for(Scheme* s: schemes)
        {
            writeSchemePart(s, currentLevel + 1);
            delete s;
        }

        delete scheme;

        writeStatistics(statistics, currentLevel);
        delete statistics;

        sendNeedNodes(currentLevel + 1, clientsNumber - 1);

        obj->deleteLater();
    });

    client.startDecomposition(scheme, clientsNumber, statistics->getData()[0][0]);
}

void GreedyDistributor::onIncomingGrid(Grid* grid, int level, Statistics* statistics)
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

        delete scheme;

        for(Grid* g: grids)
            delete g;

        if(level == initialLevel)
            sendResult(grid, level, commonStatistics);
        else
            onIncomingGrid(grid, level, commonStatistics);

        obj->deleteLater();
    }, Qt::QueuedConnection);

    client.startComposition(grids, scheme, level, commonStatistics->getData()[0][0]);
}

void GreedyDistributor::onReceivedNodes(int level)
{
    QList<Scheme*> schemes = readSchemeParts(level);

    for(int i=1; i<schemes.size(); i++)
        sendSchemePart(schemes[i], level);

    proceed(schemes[0], level);
}
