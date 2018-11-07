//
// Created by mariusz on 05.11.18.
//

#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "Graph.h"

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

Graph::Graph(int size)
        : nodes(size) {}

void Graph::addNode(int id, float x, float y) {
    if (id < 0 || id >= nodes.size())
        throw std::invalid_argument("Node ID out of bound");

    nodes[id].x = x;
    nodes[id].y = y;
}

void Graph::addEdge(int from, const Edge edge) {
    if (from < 0 || from >= nodes.size())
        throw std::invalid_argument("Node do not belong to graph");
    assert(edge.to < nodes.size() && edge.to >= 0 && "Invalid edge");

    nodes[from].edges.push_back(std::move(edge));
}

void Graph::consolidate() {
    for (auto &n : nodes) {
        std::sort(n.edges.begin(), n.edges.end(), [](Edge &lhs, auto &rhs) {
            return lhs.to <= rhs.to;
        });
    }
}
