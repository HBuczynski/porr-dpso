//
// Created by mariusz on 05.11.18.
//

#ifndef PORR_DPSO_GRAPH_H
#define PORR_DPSO_GRAPH_H

#include <ostream>
#include "Node.h"

class Graph {
public:
    explicit Graph(int size);

    friend std::ostream &operator<<(std::ostream &os, const Graph &graph);

    void addNode(int id, float x, float y);

    void addEdge(int from, const Edge edge);

    void consolidate();

private:
    std::vector<Node> nodes;
};


#endif //PORR_DPSO_GRAPH_H
