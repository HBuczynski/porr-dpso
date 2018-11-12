#include <cassert>
#include <iostream>
#include "graph/Graph.h"
#include "io/UtahGraphLoader.h"
#include "dpso/DPSO.h"

int main() {
    std::cout << "porr-dpso" << std::endl;

    auto utahGraphLoader = UtahGraphLoader("OL", 30);
    auto graph = utahGraphLoader.load();
    utahGraphLoader.show();
    graph.consolidate();

    auto dpso = DPSO(graph, 6, 26, 5, 10);
    dpso.solve();

    return 0;
}