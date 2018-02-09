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

void GeneratorParameters::setInNodeWireChance(double chance)
{
    if(chance < 0.0 || chance > 1.0)
        throw IllegalArgumentException(QObject::tr("Chance value must be in range from 0 to 1, passed: %1.")
                                       .arg(QString::number(chance)));

    this->inNodeWireChance = chance;
}

void GeneratorParameters::setNodeCapacity(int mean, double sigma, int lowerLimit, int upperLimit)
{
    validateDistributionParameters(mean, sigma, lowerLimit, upperLimit);

    nodeCapacityMean = mean;
    nodeCapacitySigma = sigma;
    nodeCapacityLowerLimit = lowerLimit;
    nodeCapacityUpperLimit = upperLimit;
}

void GeneratorParameters::setBranching(int mean, double sigma, int lowerLimit, int upperLimit)
{
    validateDistributionParameters(mean, sigma, lowerLimit, upperLimit);

    branchingMean = mean;
    branchingSigma = sigma;
    branchingLowerLimit = lowerLimit;
    branchingUpperLimit = upperLimit;
}

void GeneratorParameters::validateDistributionParameters(int mean, double sigma, int lowerLimit, int upperLimit)
{
    if(mean < 0 || sigma < 0 || lowerLimit < 0 || upperLimit < 0)
        throw IllegalArgumentException(QObject::tr("Distribution parameters cannot be negative."));

    if (upperLimit < lowerLimit)
        throw IllegalArgumentException(QObject::tr("Upper limit must be bigger or equal than lower limit."));

    if(mean < lowerLimit || mean > upperLimit)
        throw IllegalArgumentException(QObject::tr("Mean must be in limits."));
}
