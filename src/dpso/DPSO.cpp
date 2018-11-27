//
// Created by mariusz on 12.11.18.
//

#include <cassert>
#include "DPSO.h"

DPSO::DPSO(const Graph &graph, int begin, int end, const DPSOConfig config)
        : graph(graph), begin(begin), end(end), config(config) {
    auto validate_endpoints = [&graph](int id, std::string name) {
        if (id < 0 || id >= graph.size())
            throw std::invalid_argument(name + " node do not belong to graph");
    };
    validate_endpoints(begin, "Begin");
    validate_endpoints(end, "End");
}

void DPSO::solve() {
    build_swarm();
    update_best_position();
    for (auto i = 0; i < config.iterations; ++i) {
        for (auto &particle : swarm) {
            particle.calculate_velocity(best_position, config);
            particle.calculate_new_position(config);
            particle.close_new_path(graph, begin, end);
        }
        update_best_position();
    }
}

void DPSO::build_swarm() {
    assert (swarm.empty());
    for (auto i = 0; i < config.population_cnt; ++i)
        swarm.emplace_back(graph, begin, end);
}

void DPSO::update_best_position() {
    std::sort(swarm.begin(), swarm.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.best_path_length < rhs.best_path_length;
    });
    auto length = swarm.begin()->best_path_length;
    if (length < best_path_length) {
        best_path_length = length;
        best_position = swarm.begin()->best_position;
    }
}
