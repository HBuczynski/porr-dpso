//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_PARTICLE_H
#define PORR_DPSO_PARTICLE_H


#include <unordered_set>
#include "DPSOEdge.h"
#include "../graph/Graph.h"
#include "DPSOConfig.h"

using EdgesSet = std::unordered_multiset<DPSOEdge>;

struct Particle {
    friend bool operator<(const Particle &lhs, const Particle &rhs);

    Particle(const Graph &graph, NodeID from, NodeID to);

    void update_best_position();

    void calculate_velocity(EdgesSet global_best_position, const DPSOConfig &config);

    void calculate_new_position(const DPSOConfig &config);

    void close_new_path(const Graph &graph, NodeID begin, NodeID end);

    EdgesSet position;
    EdgesSet best_position;
    float best_path_length;
    EdgesSet velocity;

private:
    void build_first_path(const Graph &graph);
    float calculate_path_length();
    std::vector<DPSOEdge> build_ordered_path(const Graph &graph, NodeID end) const;
};


#endif //PORR_DPSO_PARTICLE_H
