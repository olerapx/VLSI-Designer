#ifndef ROWWISEPLACEMENT_H
#define ROWWISEPLACEMENT_H

#include "algorithms/placement/primaryplacementalgorithm.h"

/**
 * @brief The RowWisePlacement class
 * Performs the row-wise placement.
 * All elements will be grouped by their height and placed in rows.
 */
class RowWisePlacement: public PrimaryPlacementAlgorithm
{
public:
    RowWisePlacement(Scheme* scheme, double expandCoefficient);

    PlacementResult* execute();

private:

};

#endif // ROWWISEPLACEMENT_H
