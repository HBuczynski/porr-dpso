//
// Created by mariusz on 05.11.18.
//

#include "Edge.h"

bool operator==(const Edge &lhs, const Edge &rhs) {
    if (lhs.weight != rhs.weight)
        return false;
    if (lhs.from == rhs.from && lhs.to == rhs.to)
        return true;
    return lhs.from == rhs.to && lhs.to == rhs.from;
}

Edge::Edge(NodeID from, NodeID to, float weight)
        : from(from), to(to), weight(weight) {}
