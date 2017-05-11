#ifndef GENERATORPARAMETERS_H
#define GENERATORPARAMETERS_H

#include <QList>
#include "datamodels/library/library.h"
#include "datamodels/scheme/scheme.h"
#include "exception/illegalargumentexception.h"

class GeneratorParameters
{
private:
    int elementsNumber;

    int nodeCapacityMean;
    double nodeCapacitySigma;
    int nodeCapacityLeftRange;
    int nodeCapacityRightRange;

    int branchingMean;
    double branchingSigma;
    int branchingLeftRange;
    int branchingRightRange;

    QList<Library*> libraries;

    void validateDistributionParameters(int mean, double sigma, int leftRange, int rightRange);

public:
    GeneratorParameters(QList<Library*> libraries);

    void setElementsNumber(int number);

    void setNodeCapacity(int mean, double sigma, int leftRange, int rightRange);
    void setBranching(int mean, double sigma, int leftRange, int rightRange);


    int getElementsNumber() const { return elementsNumber; }

    int getNodeCapacityMean() const { return nodeCapacityMean; }
    int getNodeCapacitySigma() const { return nodeCapacitySigma; }
    int getNodeCapacityLeftRange() const { return nodeCapacityLeftRange; }
    int getNodeCapacityRightRange() const { return nodeCapacityRightRange; }

    int getBranchingMean() const { return branchingMean; }
    int getBranchingSigma() const { return branchingSigma; }
    int getBranchingLeftRange() const { return branchingLeftRange; }
    int getBranchingRightRange() const { return branchingRightRange; }

    QList<Library*>& getLibraries() { return libraries; }
};

#endif // GENERATORPARAMETERS_H
