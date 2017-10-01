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
    level(0),
    stopped(true)
{

}

Client::~Client()
{
    sendStop();
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

void Client::startDecomposition(Scheme* scheme, int number)
{
    if(!stopped) return;

    stopped = false;

    decomposition = DecompositionStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getDecompositionAlgorithmIndex(),
                                                           scheme, number);
    moveAlgorithmToThread(decomposition);

    connect(decomposition, &DecompositionAlgorithm::sendResult, this, [this] (QList<Scheme*> list)
    {
        sendDecomposedSchemes(list);
        stopped = true;
    });

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

void Client::startPlacingAndRouting(Scheme* scheme)
{
    if(!stopped) return;

    stopped = false;

    this->scheme = scheme;

    primaryPlacement = PrimaryPlacementStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getPrimaryPlacementAlgorithmIndex(),
                                                                  scheme, libraries, architecture->getAlgorithmParameters().getExpandingCoefficient());

    moveAlgorithmToThread(primaryPlacement);

    connect(primaryPlacement, &PrimaryPlacementAlgorithm::sendResult, this, &Client::onPrimaryPlacementFinished);

    connect(primaryPlacement, &PrimaryPlacementAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

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

    connect(secondaryPlacement, &SecondaryPlacementAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete secondaryPlacement;
        secondaryPlacement = nullptr;

        sendLog(tr("Secondary placement is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}

void Client::onSecondaryPlacementFinished(PlacementResult* res)
{
    routing = RoutingStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getRoutingAlgorithmIndex(),
                                                res->getGrid(), scheme, architecture->getAlgorithmParameters().getMaxExtensionAttempts());

    delete res;
    moveAlgorithmToThread(routing);

    connect(routing, &RoutingAlgorithm::sendResult, this, [this](Grid* grid)
    {
        sendRoutedGrid(grid);
        stopped = true;
    });

    connect(routing, &RoutingAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete routing;
        routing = nullptr;

        sendLog(tr("Routing is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}

void Client::startComposition(QList<Grid*> grids, Scheme *scheme, int level)
{
    if(!stopped) return;

    stopped = false;

    this->scheme = scheme;
    this->level = level;

    composition = CompositionStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getCompositionAlgorithmIndex(),
                                                        grids, scheme);
    moveAlgorithmToThread(composition);

    connect(composition, &CompositionAlgorithm::sendResult, this, &Client::onCompositionFinished);

    connect(composition, &CompositionAlgorithm::sendFinish, this, [this, grids] ()
    {
        algorithmThread.quit();

        for(Grid* g: grids)
            delete g;

        delete composition;
        composition = nullptr;

        sendLog(tr("Composition is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}

void Client::onCompositionFinished(Grid* grid)
{
    routing = RoutingStrategy().createAlgorithm(architecture->getAlgorithmIndexes().getRoutingAlgorithmIndex(),
                                                grid, scheme, architecture->getAlgorithmParameters().getMaxExtensionAttempts());

    moveAlgorithmToThread(routing);

    connect(routing, &RoutingAlgorithm::sendResult, this, [this](Grid* grid)
    {
        sendComposedGrid(grid, level - 1);
        stopped = true;
    });

    connect(routing, &RoutingAlgorithm::sendFinish, this, [this] ()
    {
        algorithmThread.quit();

        delete routing;
        routing = nullptr;

        sendLog(tr("Routing is finished."), LogType::Success);
    }, Qt::DirectConnection);

    algorithmThread.start();
}
