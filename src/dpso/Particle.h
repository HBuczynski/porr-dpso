//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_PARTICLE_H
#define PORR_DPSO_PARTICLE_H


#include <unordered_set>
#include "DPSOEdge.h"
#include "../graph/Graph.h"

using EdgesSet = std::unordered_set<DPSOEdge>;

struct Particle {
    friend bool operator<(const Particle &lhs, const Particle &rhs);

    Particle(const Graph &graph, int from, int to);

    void update_best();

    float calculate_path_length();

    EdgesSet edges;
    EdgesSet best_path;
    float best_path_length;
};


#endif //PORR_DPSO_PARTICLE_H
