#ifndef LEEROUTING_H
#define LEEROUTING_H

#include "algorithms/routing/routingalgorithm.h"

/**
 * @brief The LeeRouting class
 * Implements the Lee path connection algorithm.
 */
class LeeRouting : public RoutingAlgorithm
{
public:
    LeeRouting(Grid* grid, Scheme* scheme);

    /**
     * @brief execute
     * Executes the algorithm with the given parameters.
     * @throw Exception
     * @return
     */
    Grid* execute();

private:
    void clear();
};

#endif // LEEROUTING_H
