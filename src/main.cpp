#include <cassert>
#include <iostream>
#include "graph/Graph.h"
#include "UtahGraphLoader.h"

int main() {
    std::cout << "porr-dpso" << std::endl;

    auto graph = UtahGraphLoader("OL", 50).load();
    graph.consolidate();
    std::cout << graph;

    return 0;
}