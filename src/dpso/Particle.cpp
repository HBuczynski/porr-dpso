//
// Created by mariusz on 12.11.18.
//

#include "Particle.h"
#include <random>

bool operator<(const Particle &lhs, const Particle &rhs) {
    return lhs.best_path_length < rhs.best_path_length;
}

Particle::Particle(const Graph &graph, int from, const int to)
        : edges(), best_path(), best_path_length(std::numeric_limits<float>::max()) {
    std::random_device random_device;
    std::mt19937 generator(random_device()); //Standard mersenne_twister_engine seeded with rd()

    // TODO build first random path to target
    while (from != to) {
        const auto &edges = graph.getEdges(from);
        std::uniform_int_distribution<> dis(0, static_cast<int>(edges.size() - 1));
        auto next_edge_idx = dis(generator);
        auto &edge = edges[next_edge_idx];
        this->edges.insert({1.0, edge});

        from = edge.to;
    }

    update_best();
}

void Particle::update_best() {
    auto path_length = calculate_path_length();
    if (path_length < best_path_length) {
        best_path_length = path_length;
        best_path = edges;
    }
}

float Particle::calculate_path_length() {
    float path_length = 0.0f;
    for (auto const&[k, v] : edges) {
        path_length += v.weight;
    }
    return path_length;
}
