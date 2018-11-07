#include <cassert>
#include <iostream>
#include "graph/Graph.h"
#include "UtahGraphLoader.h"

int main() {
    std::cout << "porr-dpso" << std::endl;

    auto utahGraphLoader = UtahGraphLoader("OL", 1000);
    auto graph = utahGraphLoader.load();
    utahGraphLoader.show();
    graph.consolidate();

    return 0;
}