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

    /**
     * @brief generate
     * Starts the process of generation. Can run in separate thread.
     * When the scheme will be generated, the sendScheme signal will be emitted.
     */
    Scheme *generate();

    /**
     * @brief isStopped
     * Use this to safely dispose the working thread.
     * @return whether the generator is stopped or not.
     */
    bool isStopped() { return actuallyStopped; }

    /**
     * @brief getParam
     * @return the parameters of this generator.
     */
    GeneratorParameters getParameters() const { return param; }

signals:
    /**
     * @brief sendScheme
     * Emits when the generated scheme is ready.
     * @param s - the generated scheme.
     */
    void sendScheme(Scheme* s);

    /**
     * @brief sendFinish
     * Emits when the generation is finished or interrupted because of any reason.
     * From that moment on, the generator will be stopped.
     */
    void sendFinish();

    /**
     * @brief sendError
     * Emits when an exception is occurred.
     * @param error - the exception text.
     */
    void sendError(QString error);

    /**
     * @brief sendLog
     * @param log
     */
    void sendLog(QString log);

public slots:

    /**
     * @brief onStart
     * Starts the generator. An alternate way is to call generate().
     */
    void onStart();

    /**
     * @brief onStop
     * Requests the generator to stop. When the generator will be stopped, a sendFinish will be emitted.
     */
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

    int getTruncatedDistributedValue(std::normal_distribution<> &dist, int lowerLimit, int upperLimit);
    SchemeElement getRandomElement();

    void generateWires();

    void checkBranching();
    qint64 countAllInputPins();

    LibraryElement getCorrespondingElement(SchemeElement element);

    void generateWiresForOutput(NodeElement &element, Pin p);

    void generateOuterWire(NodeElement &element, Pin p);
    bool tryGenerateInnerWire(NodeElement &element, Pin p, int attempts);

    std::pair<NodeElement, Pin> getRandomPin(int node = freeNodeElementIndex);

    bool isWireExist(NodeElement sourceElement, Wire other);
    Wire buildWire(NodeElement sourceElement, Pin sourcePin, NodeElement destElement, Pin destPin, WireType type);
};

#endif // GENERATOR_H
