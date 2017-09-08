#pragma once

#include "algorithms/composition/compositionalgorithm.h"

/**
 * @brief The PermutationComposition class
 * Performs the composition of the given grid parts into one grid.
 */
class PermutationComposition : public CompositionAlgorithm
{
public:
    PermutationComposition(QList<Grid*> grids);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @throw Exception
     * @return composed grid.
     */
    Grid* execute();

private:
    void clear();
};
