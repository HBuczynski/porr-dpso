//
// Created by mariusz on 12.11.18.
//

#include <cassert>
#include <iostream>

#include "../utility/Profiler.h"
#include "DPSO.h"

DPSO::DPSO(const Graph &graph, NodeID begin, NodeID end, DPSOConfig config)
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

    Profiler &profiler = Profiler::getInstance();
    profiler.registerParallelisationStartPoint();

    for (auto i = 0; i < config.iterations; ++i) {
        if (config.detailLogs) {
            std::cout << "Iteration=" << i + 1 << "/" << config.iterations << " gBest= " << best_path_length << "\n";
        }

#ifdef MODE_OPEN_MP
#pragma omp parallel num_threads(profiler.getThreadNumber())
#pragma omp for
#endif
        for (uint32_t p = 0; p < swarm.size(); ++p) {
            Particle &particle = swarm.at(p);
            particle.calculate_velocity(best_position, config);
            particle.calculate_new_position(config);
            particle.close_new_path(graph, begin, end);
        }
        update_best_position();
    }

    profiler.registerParallelisationStopPoint();
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

const EdgesSet &DPSO::get_best_position() const {
    return best_position;
}
