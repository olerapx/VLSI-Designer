#include "generatorparameters.h"

GeneratorParameters::GeneratorParameters(QList<Library*> libraries)
{
    if(libraries.empty())
        throw IllegalArgumentException("A library cannot be empty.");

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

void GeneratorParameters::setNodeCapacity(int mean, double sigma, int leftRange, int rightRange)
{
    validateDistributionParameters(mean, sigma, leftRange, rightRange);

    nodeCapacityMean = mean;
    nodeCapacitySigma = sigma;
    nodeCapacityLeftRange = leftRange;
    nodeCapacityRightRange = rightRange;
}

void GeneratorParameters::setBranching(int mean, double sigma, int leftRange, int rightRange)
{
    validateDistributionParameters(mean, sigma, leftRange, rightRange);

    branchingMean = mean;
    branchingSigma = sigma;
    branchingLeftRange = leftRange;
    branchingRightRange = rightRange;
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
