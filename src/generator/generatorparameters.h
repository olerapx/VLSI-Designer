#ifndef GENERATORPARAMETERS_H
#define GENERATORPARAMETERS_H

#include <QList>
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

    int nodeCapacityMean;
    double nodeCapacitySigma;
    int nodeCapacityLeftLimit;
    int nodeCapacityRightLimit;

    int branchingMean;
    double branchingSigma;
    int branchingLeftLimit;
    int branchingRightLimit;

    QList<Library*> libraries;

    void validateDistributionParameters(int mean, double sigma, int leftRange, int rightRange);

public:
    GeneratorParameters(QList<Library*> libraries);

    void setElementsNumber(int number);

    void setNodeCapacity(int mean, double sigma, int leftLimit, int rightLimit);
    void setBranching(int mean, double sigma, int leftLimit, int rightLimit);


    int getElementsNumber() const { return elementsNumber; }

    int getNodeCapacityMean() const { return nodeCapacityMean; }
    double getNodeCapacitySigma() const { return nodeCapacitySigma; }
    int getNodeCapacityLeftLimit() const { return nodeCapacityLeftLimit; }
    int getNodeCapacityRightLimit() const { return nodeCapacityRightLimit; }

    int getBranchingMean() const { return branchingMean; }
    double getBranchingSigma() const { return branchingSigma; }
    int getBranchingLeftLimit() const { return branchingLeftLimit; }
    int getBranchingRightLimit() const { return branchingRightLimit; }

    QList<Library*>& getLibraries() { return libraries; }
};

#endif // GENERATORPARAMETERS_H
