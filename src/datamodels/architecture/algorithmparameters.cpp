#include "algorithmparameters.h"

AlgorithmParameters::AlgorithmParameters()
{
    expandingCoefficient = 1.5;
    maxExtensionAttempts = 10;
}

void AlgorithmParameters::setExpandingCoefficient(double coefficient)
{
    if(coefficient <= 1.0)
        throw IllegalArgumentException(QObject::tr("Expanding coefficient must be bigger than 1, passed: %1.").arg(QString::number(expandingCoefficient)));

    this->expandingCoefficient = coefficient;
}

void AlgorithmParameters::setMaxExtensionAttempts(int attempts)
{
    if(attempts < 0)
        throw IllegalArgumentException(QObject::tr("Max extension attempts cannot be lesser than 0, passed: %1.")
                                       .arg(QString::number(attempts)));

    this->maxExtensionAttempts = attempts;
}

void AlgorithmParameters::setMaxExtensionAttemptsForWire(int attempts)
{
    if(attempts < 0)
        throw IllegalArgumentException(QObject::tr("Max extension attempts cannot be lesser than 0, passed: %1.")
                                       .arg(QString::number(attempts)));

    this->maxExtensionAttemptsForWire = attempts;
}

bool AlgorithmParameters::operator ==(const AlgorithmParameters& other) const
{
    if(expandingCoefficient == other.expandingCoefficient &&
            maxExtensionAttempts == other.maxExtensionAttempts &&
            maxExtensionAttemptsForWire == other.maxExtensionAttemptsForWire)
        return true;

    return false;
}
