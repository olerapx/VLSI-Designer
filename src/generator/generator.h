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
class Generator
{
public:
    Generator(GeneratorParameters param);

    Scheme generate();

private:
    GeneratorParameters param;

    std::mt19937 mt;

    std::normal_distribution<> nodeCapacityDistribution;
    std::normal_distribution<> branchingDistribution;
    std::uniform_int_distribution<int> libraryRandom;

    bool stopped;
    qint64 currentIndex;

    QList<NodeElement> generateElements();

    int getTruncatedDistributedValue(std::normal_distribution<> &dist, int leftRange, int rightRange);
    SchemeElement getRandomElement();

    QList<Wire> generateWires(QList<NodeElement> elements);

    LibraryElement getCorrespondingElement(SchemeElement element);

    void generateWiresForOutput(NodeElement element, LibraryElement el, Pin p, QList<Wire> &wires);
};

#endif // GENERATOR_H
