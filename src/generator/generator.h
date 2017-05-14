#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>
#include <chrono>
#include "generatorparameters.h"
#include "nodeelement.h"

/**
 * @brief The Generator class
 * Generates a scheme using the given parameters.
 */
class Generator: public QObject
{
    Q_OBJECT

public:
    Generator(GeneratorParameters param);
    virtual ~Generator();

    void generate();

    bool isStopped() { return actuallyStopped; }

signals:
    void sendScheme(Scheme* s);
    void sendFinish();
    void sendError(QString error);
    void sendLog(QString log);

public slots:
    void onStart();
    void onStop();

private:
    static constexpr int numbersToDiscard = 1000;
    static constexpr int freeNodeElementIndex = 0;

    GeneratorParameters param;

    std::mt19937 mt;

    std::normal_distribution<> nodeCapacityDistribution;
    std::normal_distribution<> branchingDistribution;
    std::uniform_int_distribution<int> libraryRandom;

    bool stopped;
    bool actuallyStopped;
    qint64 currentElementIndex;
    qint64 currentWireIndex;

    QList<NodeElement> elements;
    QList<QList<NodeElement>> groupedElements;
    QList<Wire> wires;

    void generateElements();

    int getTruncatedDistributedValue(std::normal_distribution<> &dist, int leftRange, int rightRange);
    SchemeElement getRandomElement();

    void generateWires();

    void checkBranching();
    qint64 countAllInputPins();

    LibraryElement getCorrespondingElement(SchemeElement element);

    void generateWiresForOutput(NodeElement &element, Pin p);

    void generateOuterWire(NodeElement &element, Pin p);
    bool generateInnerWire(NodeElement &element, Pin p, int attempts);

    std::pair<NodeElement, Pin> getRandomPin(int node = freeNodeElementIndex);

    bool isWireExist(NodeElement sourceElement, Pin sourcePin, NodeElement destElement, Pin destPin);
    Wire buildWire(NodeElement sourceElement, Pin sourcePin, NodeElement destElement, Pin destPin, WireType type);
};

#endif // GENERATOR_H
