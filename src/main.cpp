#include <cassert>
#include <iostream>
#include "graph/Graph.h"

int main() {
    std::cout << "porr-dpso" << std::endl;

    // SANDBOX
    unsigned long nodes_cnt = 4;
    Graph graph(nodes_cnt);

    graph.addEdge(0, {1, 1});
    graph.addEdge(0, {1, 2});
    graph.addEdge(1, {1, 0});
    graph.addEdge(1, {1, 2});
    graph.addEdge(2, {1, 0});
    graph.addEdge(2, {1, 1});
    graph.addEdge(2, {1, 3});
    graph.addEdge(3, {1, 2});

    std::cout << graph;

    return 0;
}