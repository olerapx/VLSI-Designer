#pragma once

#include <random>
#include <chrono>

#include "threadable.h"
#include "generatorparameters.h"
#include "nodeelement.h"
#include "util/aliaser/aliaser.h"

/**
 * @brief The Generator class
 * Generates a scheme using the given parameters.
 */
class Generator : public Threadable
{
    Q_OBJECT

public:
    Generator(GeneratorParameters param);

    void setParameters(GeneratorParameters param);

    /**
     * @brief execute
     * Starts the process of generation. When the scheme will be generated, the sendScheme signal will be emitted.
     * @throw Exception
     * @return the generated scheme.
     */
    Scheme* execute();

    /**
     * @brief getParam
     * @return the parameters of this generator.
     */
    GeneratorParameters getParameters() const { return param; }

signals:
    /**
     * @brief sendResult
     * Emits when the generated scheme is ready.
     * @param s - the generated scheme.
     */
    void sendResult(Scheme* s);

public slots:
    /**
     * @brief onStart
     * Starts the generator. An alternate way is to call execute().
     */
    void onStart();

private:
    static constexpr int numbersToDiscard = 1000;
    static constexpr int freeNodeElementIndex = 0;

    GeneratorParameters param;

    std::mt19937 mt;

    std::normal_distribution<> nodeCapacityDistribution;
    std::normal_distribution<> branchingDistribution;
    std::uniform_int_distribution<int> libraryRandom;

    qint64 currentElementIndex;
    qint64 currentWireIndex;

    QList<NodeElement*> elements;
    QList<QList<NodeElement*>> groupedElements;
    QList<Wire> wires;

    void clear();

    void generateElements();
    void addFreeElement(SchemeElement element);

    int getTruncatedDistributedValue(std::normal_distribution<>& dist, int lowerLimit, int upperLimit);
    SchemeElement getRandomElement();

    void generateWires();

    void checkBranching();
    qint64 countAllPins(PinType type);

    void generateWiresForOutput(NodeElement* element, Pin p);

    void tryGenerateOuterWire(NodeElement* element, Pin p, int attempts);
    bool tryGenerateInnerWire(NodeElement* element, Pin p, int attempts);

    std::pair<NodeElement*, Pin> getRandomPin(int node = freeNodeElementIndex);

    Wire buildWire(NodeElement* sourceElement, Pin sourcePin, NodeElement* destElement, Pin destPin, WireType type);

    void generateAliases(Scheme* scheme);
};
