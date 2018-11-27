//
// Created by mariusz on 27.11.18.
//

#ifndef PORR_DPSO_ASTAR_H
#define PORR_DPSO_ASTAR_H


#include <set>
#include "../graph/Graph.h"

class Astar {
public:
    Astar(const Graph& graph, NodeID from, NodeID to);

    std::vector<NodeID> solve();

private:
    const Graph& graph;
    NodeID from;
    NodeID to;

    std::set<NodeID> closed_set;
    std::set<NodeID> open_set;
    std::unordered_map<NodeID, NodeID> came_from;
    std::unordered_map<NodeID, float> score_from_start;
    std::unordered_map<NodeID, float> score_total_heuristic;

    std::vector<NodeID> reconstruct_path(NodeID current) const;
};


#endif //PORR_DPSO_ASTAR_H
