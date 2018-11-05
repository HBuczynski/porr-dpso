//
// Created by mariusz on 05.11.18.
//

#ifndef PORR_DPSO_GRAPH_H
#define PORR_DPSO_GRAPH_H

#include <ostream>
#include "Node.h"

class Graph {
public:
    explicit Graph(unsigned long size);

    friend std::ostream &operator<<(std::ostream &os, const Graph &graph);

    void addEdge(unsigned long id, Edge edge);

private:
    std::vector<Node> nodes;
};


#endif //PORR_DPSO_GRAPH_H
