#pragma once

#include <QObject>

#include "exception/illegalargumentexception.h"

/**
 * @brief The AlgorithmParameters class
 * Encapsulates the parameters of the algorithms used in VLSI design process.
 */
class AlgorithmParameters
{
public:
    AlgorithmParameters();

    double getExpandingCoefficient() { return expandingCoefficient; }
    void setExpandingCoefficient(double coefficient);

    int getMaxExtensionAttempts() { return maxExtensionAttempts; }
    void setMaxExtensionAttempts(int attempts);

    bool operator ==(const AlgorithmParameters& other);

private:
    double expandingCoefficient;
    int maxExtensionAttempts;
};
