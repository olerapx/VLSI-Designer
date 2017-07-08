#include "decompositionalgorithm.h"

DecompositionAlgorithm::DecompositionAlgorithm(Scheme *scheme, int number)
{
    if(scheme->getElements().size() == 0)
        throw IllegalArgumentException(tr("The scheme cannot contain 0 elements."));

    if(scheme->getWires().size() == 0)
        throw IllegalArgumentException(tr("The scheme cannot contain 0 wires."));

    if(scheme->getElements().size() < number)
        throw IllegalArgumentException(tr("The number of scheme's elements (%1) is lower than number of subschemes (%2).")
                                       .arg(QString::number(scheme->getElements().size()), QString::number(number)));

    this->scheme = scheme;
    this->number = number;

    stopped = true;
    actuallyStopped = true;
}

void DecompositionAlgorithm::onStart()
{
    execute();
}

void DecompositionAlgorithm::onStop()
{
    stopped = true;
}
