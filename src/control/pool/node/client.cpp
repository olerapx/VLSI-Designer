#include "client.h"

Client::Client() :
    scheme(nullptr),
    grid(nullptr),
    architecture(nullptr),
    decomposition(nullptr),
    primaryPlacement(nullptr),
    secondaryPlacement(nullptr),
    routing(nullptr),
    composition(nullptr),
    renderer(nullptr),
    level(0),
    stopped(true)
{

}

Client::~Client()
{
    stop();
    algorithmThread.wait();
}

void Client::stop()
{
    sendStop();
    stopped = true;

    algorithmThread.quit();
    algorithmThread.wait();
}

void Client::setArchitecture(Architecture* a)
{
    if(!stopped) return;

    delete this->architecture;
    this->architecture = a;
}

void Client::setLibraries(QList<Library*> list)
{
    if(!stopped) return;

    for(Library*l: this->libraries)
        delete l;

    this->libraries.clear();
    this->libraries = list;
}

bool Client::isInitialized() const
{
    if(architecture == nullptr || libraries.empty())
        return false;

    return true;
}

void Client::startDecomposition(Scheme* scheme, int number, StatisticsEntry& entry)
{
    if(!stopped) return;

    stopped = false;
    this->entry = &entry;

    decomposition = DecompositionStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getDecompositionAlgorithmIndex(),
                                                           scheme, number);
    moveAlgorithmToThread(decomposition);

    connect(decomposition, &DecompositionAlgorithm::sendResult, this, [this] (QList<Scheme*> list)
    {
        sendDecomposedSchemes(list);
        stopped = true;
    });

    connect(decomposition, &DecompositionAlgorithm::sendTime, this, [this] (int time)
    {
        this->entry->setDecompositionTime(time);
    }, Qt::DirectConnection);

    connect(decomposition, &DecompositionAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete decomposition;
        decomposition = nullptr;

        sendLog(tr("Decomposition is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}

void Client::moveAlgorithmToThread(Threadable* t)
{
    t->moveToThread(&algorithmThread);

    connect(&algorithmThread, &QThread::started, t, &Threadable::onStart);
    connect(this, &Client::sendStop, t, &Threadable::onStop, Qt::DirectConnection);

    connect(t, &Threadable::sendLog, this, &Client::onLog, Qt::DirectConnection);
    connect(t, &Threadable::sendError, this, &Client::sendError, Qt::DirectConnection);
}

void Client::startPlacingAndRouting(Scheme* scheme, StatisticsEntry& entry)
{
    if(!stopped) return;

    stopped = false;

    this->scheme = scheme;
    this->entry = &entry;

    primaryPlacement = PrimaryPlacementStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getPrimaryPlacementAlgorithmIndex(),
                                                                  scheme, libraries, architecture->getAlgorithmParameters().getExpandingCoefficient());

    moveAlgorithmToThread(primaryPlacement);

    connect(primaryPlacement, &PrimaryPlacementAlgorithm::sendResult, this, &Client::onPrimaryPlacementFinished);

    connect(primaryPlacement, &PrimaryPlacementAlgorithm::sendTime, this, [this] (int time)
    {
        this->entry->setPrimaryPlacementTime(time);
    }, Qt::DirectConnection);

    connect(primaryPlacement, &PrimaryPlacementAlgorithm::sendFinish, this, [this] ()
    {
        delete primaryPlacement;
        primaryPlacement = nullptr;

        sendLog(tr("Primary placement is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}

void Client::onLog(QString log)
{
    sendLog(log);
}

void Client::onPrimaryPlacementFinished(PlacementResult* res)
{
    secondaryPlacement = SecondaryPlacementStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getSecondaryPlacementAlgorithmIndex(),
                                                                      res);
    moveAlgorithmToThread(secondaryPlacement);

    connect(secondaryPlacement, &SecondaryPlacementAlgorithm::sendResult, this, &Client::onSecondaryPlacementFinished);

    connect(secondaryPlacement, &SecondaryPlacementAlgorithm::sendTime, this, [this] (int time)
    {
        entry->setSecondaryPlacementTime(time);
    }, Qt::DirectConnection);

    connect(secondaryPlacement, &SecondaryPlacementAlgorithm::sendFinish, this, [this] ()
    {
        delete secondaryPlacement;
        secondaryPlacement = nullptr;

        sendLog(tr("Secondary placement is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.quit();
    algorithmThread.wait();

    algorithmThread.start();
}

void Client::onSecondaryPlacementFinished(PlacementResult* res)
{
    routing = RoutingStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getRoutingAlgorithmIndex(),
                                                res->getGrid(), scheme, architecture->getAlgorithmParameters().getMaxExtensionAttempts(),
                                                architecture->getAlgorithmParameters().getMaxExtensionAttemptsForWire());

    delete res;
    moveAlgorithmToThread(routing);

    calculateWiresNumber(res->getGrid());

    connect(routing, &RoutingAlgorithm::sendResult, this, [this](Grid* grid)
    {
        calculateRoutedDelta(grid);

        sendRoutedGrid(grid);
        stopped = true;
    });

    connect(routing, &RoutingAlgorithm::sendTime, this, [this] (int time)
    {
        entry->setInternalRoutingTime(time);
    }, Qt::DirectConnection);

    connect(routing, &RoutingAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete routing;
        routing = nullptr;

        sendLog(tr("Routing is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.quit();
    algorithmThread.wait();

    algorithmThread.start();
}

void Client::calculateWiresNumber(Grid* grid)
{
    int internalUnroutedWires = 0;
    int totalUnroutedWires = 0;

    for(WireData& data: grid->getWiresData())
    {
        if(grid->getRoutedWires().contains(data.getIndex()))
            continue;

        if(data.getWirePosition() == WirePosition::Internal)
            internalUnroutedWires ++;

        totalUnroutedWires ++;
    }

    entry->setInternalUnroutedWiresNumber(internalUnroutedWires);
    entry->setTotalUnroutedWiresNumber(totalUnroutedWires);
}

void Client::calculateRoutedDelta(Grid* grid)
{
    int oldInternalUnroutedWires = entry->getInternalUnroutedWiresNumber();
    int newInternalUnroutedWires = 0;

    for(WireData& data: grid->getWiresData())
    {
        if(grid->getRoutedWires().contains(data.getIndex()))
            continue;

        if(data.getWirePosition() == WirePosition::Internal)
            newInternalUnroutedWires ++;
    }

    entry->setRoutedWiresDelta(oldInternalUnroutedWires - newInternalUnroutedWires);
}

void Client::startComposition(QList<Grid*> grids, Scheme* scheme, int level, StatisticsEntry& entry)
{
    if(!stopped) return;

    stopped = false;

    this->scheme = scheme;
    this->level = level;
    this->entry = &entry;

    composition = CompositionStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getCompositionAlgorithmIndex(),
                                                        grids, scheme);
    moveAlgorithmToThread(composition);

    connect(composition, &CompositionAlgorithm::sendResult, this, &Client::onCompositionFinished);

    connect(composition, &CompositionAlgorithm::sendTime, this, [this, grids] (int time)
    {
        this->entry->setCompositionTime(time);
    }, Qt::DirectConnection);

    connect(composition, &CompositionAlgorithm::sendFinish, this, [this, grids] ()
    {
        delete composition;
        composition = nullptr;

        sendLog(tr("Composition is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}

void Client::onCompositionFinished(Grid* grid)
{
    routing = RoutingStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getRoutingAlgorithmIndex(),
                                                grid, scheme, architecture->getAlgorithmParameters().getMaxExtensionAttempts(),
                                                architecture->getAlgorithmParameters().getMaxExtensionAttemptsForWire());

    moveAlgorithmToThread(routing);

    calculateWiresNumber(grid);

    connect(routing, &RoutingAlgorithm::sendResult, this, [this](Grid* grid)
    {
        calculateRoutedDelta(grid);

        sendComposedGrid(grid, level - 1);
        stopped = true;
    });

    connect(routing, &RoutingAlgorithm::sendTime, this, [this] (int time)
    {
        entry->setExternalRoutingTime(time);
    }, Qt::DirectConnection);

    connect(routing, &RoutingAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete routing;
        routing = nullptr;

        sendLog(tr("Routing is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.quit();
    algorithmThread.wait();

    algorithmThread.start();
}

void Client::startRendering(Grid* grid, Scheme* scheme)
{
    if(!stopped) return;

    stopped = false;

    renderer = new GridRenderer(grid, scheme);
    moveAlgorithmToThread(renderer);

    connect(renderer, &GridRenderer::sendResult, this, [this] (QImage image)
    {
        sendGridImage(image);
        stopped = true;
    });

    connect(renderer, &GridRenderer::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete renderer;
        renderer = nullptr;

        sendLog(tr("Rendering is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}
