#include "generatorparameters.h"

GeneratorParameters::GeneratorParameters(QList<Library*> libraries)
{
    if(libraries.empty())
        throw IllegalArgumentException("Library cannot be empty.");

    this->libraries = libraries;

    setElementsNumber(10000);

    setNodeCapacity(6, 2, 2, 7);
    setBranching(4, 0.5, 1, 4);
}

void GeneratorParameters::setElementsNumber(int number)
{
    if (number <= 0)
        throw IllegalArgumentException("Number of elements must be positive.");

    this->elementsNumber = number;
}

void GeneratorParameters::setInnerWireChance(double chance)
{
    if(chance < 0.0 || chance > 1.0)
        throw IllegalArgumentException("Invalid chance value");

    this->innerWireChance = chance;
}

void GeneratorParameters::setNodeCapacity(int mean, double sigma, int leftLimit, int rightLimit)
{
    validateDistributionParameters(mean, sigma, leftLimit, rightLimit);

    nodeCapacityMean = mean;
    nodeCapacitySigma = sigma;
    nodeCapacityLeftLimit = leftLimit;
    nodeCapacityRightLimit = rightLimit;
}

void GeneratorParameters::setBranching(int mean, double sigma, int leftLimit, int rightLimit)
{
    validateDistributionParameters(mean, sigma, leftLimit, rightLimit);

    branchingMean = mean;
    branchingSigma = sigma;
    branchingLeftLimit = leftLimit;
    branchingRightLimit = rightLimit;
}

void GeneratorParameters::validateDistributionParameters(int mean, double sigma, int leftRange, int rightRange)
{
    if(mean <= 0 || sigma <= 0 || leftRange <= 0 || rightRange <= 0)
        throw IllegalArgumentException("Distribution parameters must be positive.");

    if (rightRange < leftRange)
        throw IllegalArgumentException("Right range must be bigger or equal than left range");

    if(mean < leftRange || mean > rightRange)
        throw IllegalArgumentException("Mean must be in ranges.");
}
