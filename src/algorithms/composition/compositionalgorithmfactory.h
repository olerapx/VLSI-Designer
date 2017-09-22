#pragma once

#include <QMap>

#include "algorithms/composition/permutationcomposition/permutationcomposition.h"

static const QMap<QString, QString> algorithmsMap
{
    {"permutation", QT_TR_NOOP("Permutation composition")}
};

class CompositionAlgorithmFactory
{
public:
   static CompositionAlgorithm* createAlgorithm(QString key, QList<Grid*> grids, Scheme* scheme);

private:
   CompositionAlgorithmFactory() {}
};
