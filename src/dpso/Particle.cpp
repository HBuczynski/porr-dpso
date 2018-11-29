//
// Created by mariusz on 12.11.18.
//

#include "Particle.h"
#include "DPSOConfig.h"
#include "Astar.h"
#include <random>
#include <bits/unordered_set.h>
#include <unordered_set>

bool operator<(const Particle &lhs, const Particle &rhs) {
    return lhs.best_path_length < rhs.best_path_length;
}

Particle::Particle(const Graph &graph, const NodeID from, const NodeID to)
        : position(), best_position(), best_path_length(std::numeric_limits<float>::max()) {
    build_first_path(graph);
    close_new_path(graph, from, to);
}

void Particle::build_first_path(const Graph &graph) {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> random_node(0, graph.size() - 1);

    for (auto i = 0; i < 10; ++i) {
        auto node_id = static_cast<NodeID>(random_node(generator));
        const auto &edges = graph.getEdges(node_id);

        auto edge_idx = random_node(generator) % edges.size();
        auto edge = edges[edge_idx];
        if (position.find({edge, 1.0}) == position.end())
            position.insert({edge, 1.0});
    }
}

void Particle::update_best_position() {
    auto path_length = calculate_path_length();
    if (path_length < best_path_length) {
        best_path_length = path_length;
        best_position = position;
    }
}

float Particle::calculate_path_length() {
    float path_length = 0.0f;
    for (auto const &e : position) {
        path_length += e.edge.weight;
    }
    return path_length;
}

void Particle::calculate_velocity(EdgesSet global_best_position, const DPSOConfig &config) {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_real_distribution<> random(0.0, 1.0);

    // inertion
    // temporary variable due to const iterator in for loop
    EdgesSet new_velocity{};
    for (auto &v : velocity)
        new_velocity.insert(v * config.swarm_inertion);
    velocity = new_velocity;

    // social
    for (const auto &e : position)
        global_best_position.erase(e);
    for (const auto &e : global_best_position)
        velocity.insert(e * config.social_coefficient * random(generator));

    // cognitive
    auto particle_best_position = best_position;
    for (const auto &e : position)
        particle_best_position.erase(e);
    for (const auto &e : particle_best_position)
        velocity.insert(e * config.cognitive_coefficient * random(generator));
}

void Particle::calculate_new_position(const DPSOConfig &config) {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_real_distribution<> random(0.0, 1.0);

    EdgesSet new_position{};

    // Filter edges from velocity
    for (const auto &e : velocity) {
        if (random(generator) <= e.propability)
            new_position.insert(e);
    }

    // Filter edges from current position
    for (const auto &e : position) {
        if (random(generator) <= random(generator) * config.previous_pos_impact_coefficient)
            new_position.insert(e);
    }
    position = new_position;
}

void Particle::close_new_path(const Graph &graph, NodeID begin, NodeID end) {
    auto g = graph;
    for (const auto de : position) {
        g.change_edge_weight(de.edge.from, de.edge.to, 0.0f);
        g.change_edge_weight(de.edge.to, de.edge.from, 0.0f);
    }

    // TODO this alg in future should be changed to sth more random
    auto path_finder = Astar(g, begin, end);
    auto path = path_finder.solve();

    for (auto i = 1u; i < path.size(); ++i) {
        auto from = path[i - 1];
        auto to = path[i];

        const auto &edges = graph.getEdges(from);
        auto edge = std::find_if(edges.begin(), edges.end(), [to](const auto &obj) {
            return obj.to == to;
        });

        position.insert(DPSOEdge{Edge{from, to, edge->weight}, 1.0});
    }

    update_best_position();
}
