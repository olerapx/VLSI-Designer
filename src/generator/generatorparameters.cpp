#include "generatorparameters.h"

GeneratorParameters::GeneratorParameters(QList<Library*> libraries)
{
    if(libraries.empty())
        throw IllegalArgumentException(QObject::tr("Library cannot be empty."));

    this->libraries = libraries;

    setElementsNumber(10000);

    setNodeCapacity(6, 2, 2, 7);
    setBranching(4, 0.5, 1, 4);
}

void GeneratorParameters::setElementsNumber(int number)
{
    if (number <= 0)
        throw IllegalArgumentException(QObject::tr("Number of elements must be positive, passed: %1.")
                                       .arg(QString::number(number)));

    this->elementsNumber = number;
}

void GeneratorParameters::setInnerWireChance(double chance)
{
    if(chance < 0.0 || chance > 1.0)
        throw IllegalArgumentException(QObject::tr("Chance value must be in range from 0 to 1, passed: %1.")
                                       .arg(QString::number(chance)));

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

void GeneratorParameters::validateDistributionParameters(int mean, double sigma, int leftLimit, int rightLimit)
{
    if(mean <= 0 || sigma <= 0 || leftLimit <= 0 || rightLimit <= 0)
        throw IllegalArgumentException(QObject::tr("Distribution parameters must be positive."));

    if (rightLimit < leftLimit)
        throw IllegalArgumentException(QObject::tr("Right limit must be bigger or equal than left limit."));

    if(mean < leftLimit || mean > rightLimit)
        throw IllegalArgumentException(QObject::tr("Mean must be in limits."));
}
