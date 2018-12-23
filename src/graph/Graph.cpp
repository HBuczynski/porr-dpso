//
// Created by mariusz on 05.11.18.
//

#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include "Graph.h"

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
    if (graph.nodes.empty()) {
        os << "Graph is empty\n";
        return os;
    }

    os << "ID | Neighbours\n";
    for (auto id = 0u; id < graph.nodes.size(); id++) {
        os << id << " -> ";
        for (auto edge : graph.nodes[id].edges)
            os << edge.to << " ";
        os << "\n";
    }
    return os;
}

Graph::Graph(int size)
        : nodes(size) {}

void Graph::addNode(NodeID id, float x, float y) {
    if (id >= nodes.size())
        throw std::invalid_argument("Node ID out of bound");

    nodes[id].x = x;
    nodes[id].y = y;
}

void Graph::addEdge(const Edge edge) {
    if (edge.from >= nodes.size())
        throw std::invalid_argument("Node do not belong to graph");
    assert(edge.to < nodes.size() && "Invalid edge");

    nodes[edge.from].edges.push_back(std::move(edge));
}

void Graph::consolidate() {
    for (auto &n : nodes) {
        std::sort(n.edges.begin(), n.edges.end(), [](Edge &lhs, auto &rhs) {
            return lhs.to <= rhs.to;
        });
    }
}

int Graph::size() const {
    assert(nodes.size() < INT32_MAX && "Reimplement Graph::size()");
    return static_cast<int>(nodes.size());
}

const Node &Graph::getNode(NodeID id) const {
    assert(id < nodes.size() && "ID do not belong to graph");
    return nodes[id];
}

const std::vector<Edge> &Graph::getEdges(const NodeID id) const {
    assert(id < nodes.size() && "ID do not belong to graph");
    return nodes[id].edges;
}

void Graph::change_edge_weight(NodeID from, NodeID to, float weight) {
    auto &edges = nodes[from].edges;
    const auto &it = std::find_if(edges.begin(), edges.end(), [to](const auto &e) {
        return e.to == to;
    });
    if (it == edges.end())
        throw std::domain_error("Edge should exist");

    it->weight = weight;
}

float Graph::straight_line(NodeID from, NodeID to) const {
    auto dx2 = pow(nodes[from].x - nodes[to].x, 2.0);
    auto dy2 = pow(nodes[from].y - nodes[to].y, 2.0);
    return static_cast<float>(sqrt(dx2 + dy2));
}
