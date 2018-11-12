//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_DPSO_H
#define PORR_DPSO_DPSO_H


#include <set>
#include "../graph/Graph.h"
#include "Particle.h"

using Swarm = std::set<Particle>;

class DPSO {
public:
    DPSO(const Graph &graph, int begin, int end, int population_cnt, int iterations);

    void solve();

private:

    const Graph graph;
    Swarm swarm;
    EdgesSet best_path;
    float best_path_length = std::numeric_limits<float>::max();


    void build_swarm();
    void update_best();

    const int begin;
    const int end;
    const int population_cnt;
    const int iterations;
};


#endif //PORR_DPSO_DPSO_H
