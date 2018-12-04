//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_DPSO_H
#define PORR_DPSO_DPSO_H


#include <set>
#include "../graph/Graph.h"
#include "Particle.h"
#include "DPSOConfig.h"

using Swarm = std::vector<Particle>;

class DPSO {
public:
    DPSO(const Graph &graph, NodeID begin, NodeID end, DPSOConfig config);

    void solve();

private:
    const Graph graph;
    Swarm swarm;
    EdgesSet best_position;
public:
    const EdgesSet &getBest_position() const;

private:
    float best_path_length = std::numeric_limits<float>::max();

    void build_swarm();

    void update_best_position();

    NodeID begin;
    NodeID end;
    const DPSOConfig config;
};


#endif //PORR_DPSO_DPSO_H
