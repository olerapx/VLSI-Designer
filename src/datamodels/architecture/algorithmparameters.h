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

    double getExpandingCoefficient() const { return expandingCoefficient; }
    void setExpandingCoefficient(double coefficient);

    int getMaxExtensionAttempts() const { return maxExtensionAttempts; }
    void setMaxExtensionAttempts(int attempts);

    int getMaxExtensionAttemptsForWire() const { return maxExtensionAttemptsForWire; }
    void setMaxExtensionAttemptsForWire(int attempts);

    bool operator ==(const AlgorithmParameters& other) const;

private:
    double expandingCoefficient;
    int maxExtensionAttempts;
    int maxExtensionAttemptsForWire;
};
