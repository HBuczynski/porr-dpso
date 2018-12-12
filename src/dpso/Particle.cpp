//
// Created by mariusz on 12.11.18.
//

#include "Particle.h"
#include "DPSOConfig.h"
#include "Astar.h"
#include <random>
#include <bits/unordered_set.h>
#include <unordered_set>
#include <cassert>
#include <iostream>

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
    EdgesSet new_velocity;
    for (auto &v : velocity)
        new_velocity.insert(v * config.swarm_inertion);
    velocity = std::move(new_velocity);

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

    EdgesSet new_position;

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

    // remove duplicates, leaving only the highest probability
    position.clear();
    if (new_position.empty())
        return;

    auto current_best = *new_position.begin();
    static_assert(std::is_same<EdgesSet, std::unordered_multiset<DPSOEdge>>::value);
    for (const auto &e : new_position) {
        if (current_best.edge == e.edge) {
            if (current_best.propability < e.propability)
                current_best = e;
        } else {
            position.insert(current_best);
            current_best = e;
        }
    }
    if (position.find(current_best) == position.end())
        position.insert(current_best);
}

std::vector<DPSOEdge> Particle::build_ordered_path(const Graph &graph, const NodeID end) const {
    auto pos = std::vector<DPSOEdge>();
    pos.reserve(position.size());
    for (const auto &p : position)
        pos.push_back(p);

    auto distance_to_end = [&graph, end](DPSOEdge edge) {
        auto d1 = graph.straight_line(end, edge.edge.from);
        auto d2 = graph.straight_line(end, edge.edge.to);
        return d1 < d2 ? d1 : d2;
    };

    std::sort(pos.begin(), pos.end(), [&distance_to_end](auto lhs, auto rhs) {
        auto d1 = distance_to_end(lhs);
        auto d2 = distance_to_end(rhs);
        return d1 > d2;
    });
    return pos;
}

void Particle::close_new_path(const Graph &graph, const NodeID begin, NodeID end) {
    auto pos = build_ordered_path(graph, end);

    while (true) {
        auto last_iteration = false;
        auto current_from = end;
        auto current_to = begin;

        if (!pos.empty()) {
            DPSOEdge current = pos.back();
            pos.pop_back();

            assert(current.edge.from < static_cast<NodeID>(graph.size()) && "Domain constrain has been broken");
            assert(current.edge.to < static_cast<NodeID>(graph.size()) && "Domain constrain has been broken");
            current_from = current.edge.from;
            current_to = current.edge.to;
        } else {
            last_iteration = true;
        }

        auto path_finder = Astar(graph, end, current_to);
        auto path_patch = path_finder.solve();

        for (auto i = 1u; i < path_patch.size(); ++i) {
            auto from = path_patch[i - 1];
            auto to = path_patch[i];

            const auto &edges = graph.getEdges(from);
            auto edge = std::find_if(edges.begin(), edges.end(), [to](const Edge &edge) {
                return edge.to == to;
            });
            auto new_edge = DPSOEdge{Edge{from, to, edge->weight}, 1.0};

            if (position.find(new_edge) == position.end())
                position.insert(new_edge);
        }

        if (last_iteration)
            break;

        end = current_from;
    }

    update_best_position();
}
