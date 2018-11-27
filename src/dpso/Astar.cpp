//
// Created by mariusz on 27.11.18.
//

#include "Astar.h"

Astar::Astar(const Graph &graph, NodeID from, NodeID to)
        : graph(graph), from(from), to(to) {
    open_set.insert(from);

    score_from_start.rehash(static_cast<unsigned long>(graph.size()));
    score_from_start.insert({from, 0.0});

    score_total_heuristic.rehash(static_cast<unsigned long>(graph.size()));
    for (auto id = 0; id < graph.size(); ++id)
        score_total_heuristic.insert({static_cast<const NodeID &>(id), std::numeric_limits<float>::max()});
    score_total_heuristic.insert({from, graph.straight_line(from, to)});
}

std::vector<NodeID> Astar::solve() {
    while (!open_set.empty()) {
        // TODO sort by score_total_heuristic, current = n with lowest score_total_heuristic
        auto current = *open_set.begin();
        if (current == to)
            return reconstruct_path(current);

        open_set.erase(current);
        closed_set.insert(current);

        for (const auto &edge_out : graph.getEdges(current)) {
            auto neighbour = edge_out.to;

            if (closed_set.count(neighbour) != 0)
                continue;

            auto distance_from_start_to_neighbour = score_from_start[current] + edge_out.weight;
            if (open_set.count(neighbour) == 0)
                open_set.insert(neighbour);
            else if (distance_from_start_to_neighbour >= score_from_start[neighbour])
                continue;

            came_from.insert({neighbour, current});
            score_from_start.insert({neighbour, distance_from_start_to_neighbour});
            score_total_heuristic.insert(
                    {neighbour, distance_from_start_to_neighbour + graph.straight_line(neighbour, to)});
        }
    }
    return {};
}

std::vector<NodeID> Astar::reconstruct_path(NodeID current) const {
    std::vector<NodeID> path;

    path.push_back(current);
    while (came_from.find(current) != came_from.end()) {
        current = came_from.at(current);
        path.push_back(current);
    }
    return path;
}
