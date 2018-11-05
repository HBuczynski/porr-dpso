//
// Created by mariusz on 05.11.18.
//

#ifndef PORR_DPSO_NODE_H
#define PORR_DPSO_NODE_H

#include <vector>
#include "Edge.h"

struct Node {
    float x;
    float y;

    std::vector<Edge> edges;
};


#endif //PORR_DPSO_NODE_H
