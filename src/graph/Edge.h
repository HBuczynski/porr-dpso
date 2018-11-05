//
// Created by mariusz on 05.11.18.
//

#ifndef PORR_DPSO_EDGE_H
#define PORR_DPSO_EDGE_H

using NodeID = int;

struct Edge {
    float weight;
    NodeID to;

    Edge(float weight, NodeID to);
};


#endif //PORR_DPSO_EDGE_H
