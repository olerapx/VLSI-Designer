#ifndef GENERATORPARAMETERS_H
#define GENERATORPARAMETERS_H

#include "datamodels/library/library.h"
#include "datamodels/scheme/scheme.h"
#include "exception/illegalargumentexception.h"

/**
 * @brief The GeneratorParameters class
 * Encapsulates the parameters for scheme generator.
 */
class GeneratorParameters
{
private:
    int elementsNumber;

    double innerWireChance;

    int nodeCapacityMean;
    double nodeCapacitySigma;
    int nodeCapacityLowerLimit;
    int nodeCapacityUpperLimit;

    int branchingMean;
    double branchingSigma;
    int branchingLowerLimit;
    int branchingUpperLimit;

    QList<Library*> libraries;

    void validateDistributionParameters(int mean, double sigma, int lowerLimit, int upperLimit);

public:
    GeneratorParameters(QList<Library*> libraries);

    void setElementsNumber(int number);

    void setInnerWireChance(double chance);

    void setNodeCapacity(int mean, double sigma, int lowerLimit, int upperLimit);
    void setBranching(int mean, double sigma, int lowerLimit, int upperLimit);


    int getElementsNumber() const { return elementsNumber; }

    double getInnerWireChance() { return innerWireChance; }

    int getNodeCapacityMean() const { return nodeCapacityMean; }
    double getNodeCapacitySigma() const { return nodeCapacitySigma; }
    int getNodeCapacityLowerLimit() const { return nodeCapacityLowerLimit; }
    int getNodeCapacityUpperLimit() const { return nodeCapacityUpperLimit; }

    int getBranchingMean() const { return branchingMean; }
    double getBranchingSigma() const { return branchingSigma; }
    int getBranchingLowerLimit() const { return branchingLowerLimit; }
    int getBranchingUpperLimit() const { return branchingUpperLimit; }

    QList<Library*>& getLibraries() { return libraries; }
};

#endif // GENERATORPARAMETERS_H
