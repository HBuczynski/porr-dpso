//
// Created by mariusz on 12.11.18.
//

#include <cassert>
#include "DPSO.h"

DPSO::DPSO(const Graph &graph, int begin, int end, int population_cnt, int iterations)
        : graph(graph), begin(begin), end(end), population_cnt(population_cnt), iterations(iterations) {
    auto validate_endpoints = [&graph](int id, std::string name) {
        if (id < 0 || id >= graph.size())
            throw std::invalid_argument(name + " node do not belong to graph");
    };
    validate_endpoints(begin, "Begin");
    validate_endpoints(end, "End");
}

void DPSO::solve() {
    build_swarm();
    update_best();
    for (auto i = 0; i < iterations; ++i) {
        // TODO calculate new Velocity
        // TODO calculate new Position
        // TODO make path complete
        ;
    }
}

void DPSO::build_swarm() {
    assert (swarm.empty());
    for (auto i = 0; i < population_cnt; ++i)
        swarm.emplace(graph, begin, end);
}

void DPSO::update_best() {
    auto length = swarm.begin()->best_path_length;
    if (length < best_path_length) {
        best_path_length = length;
        best_path = swarm.begin()->best_path;
    }
}
