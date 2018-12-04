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

    auto dpso_config = DPSOConfig();
    auto solver = DPSO(graph, 6, 26, dpso_config);
    solver.solve();
    utahGraphLoader.show_results(solver.getBest_position());

    return 0;
}
