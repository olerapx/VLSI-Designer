#ifndef ROWPERMUTATIONPLACEMENT_H
#define ROWPERMUTATIONPLACEMENT_H

#include "algorithms/placement/secondaryplacementalgorithm.h"

/**
 * @brief The RowPermutationPlacement class
 * Performs the row permutation and the element permutation within each row.
 * All elements must be grouped by their height and placed in rows.
 */
class RowPermutationPlacement: public SecondaryPlacementAlgorithm
{
public:
    RowPermutationPlacement(PlacementResult* previous);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @return
     */
    PlacementResult* execute();

private:
    void clear();
};

#endif // ROWPERMUTATIONPLACEMENT_H
