//
// Created by mariusz on 05.11.18.
//

#include <cassert>
#include <stdexcept>
#include "Graph.h"

Graph::Graph(unsigned long size)
        : nodes(size) {}

void Graph::addEdge(unsigned long id, const Edge edge) {
    if (id >= nodes.size() || id < 0)
        throw std::invalid_argument("id do not belong to graph");
    assert(edge.to < nodes.size() && edge.to >= 0 && "Invalid edge");

    nodes[id].edges.push_back(std::move(edge));
}

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
    if (graph.nodes.empty()) {
         os << "Graph is empty\n";
        return os;
    }

    os << "ID | Neighbours\n";
    for (auto id = 0; id < graph.nodes.size(); id++) {
        os << id << " -> ";
        for (auto edge : graph.nodes[id].edges)
            os << edge.to << " ";
        os << "\n";
    }
    return os;
}
