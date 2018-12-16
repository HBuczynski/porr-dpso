#include <cassert>
#include <iostream>
#include <fstream>

#include "graph/Graph.h"
#include "io/UtahGraphLoader.h"
#include "dpso/DPSO.h"
#include "utility/Profiler.h"

using namespace std;

int main(int argc, char *argv[]) {

    std::cout << "<< porr-dpso >>" << std::endl;
    Profiler &profiler = Profiler::getInstance();

    if (argc < 2)
    {
        cout << "You have to write test's iteration number as an input argument !!!" << endl;
        return 0;
    }
    uint16_t iterationCounter = static_cast<uint16_t >(atoi(argv[1]));

#ifdef PARARELL
    if (argc < 3)
    {
        cout << "You have to write thread number as an second input argument !!!" << endl;
        return 0;
    }
    profiler.setThreadNumber(static_cast<uint8_t >(atoi(argv[2])));
    profiler.setMode("PARARELL");
#else
    profiler.setMode("SYNCH");
#endif

    for(uint16_t i=1; i<=10; ++i)
    {
        cout << "\n## ITERATION << " << i << "/" << iterationCounter << " >>\t progressing ..." << endl;

        profiler.setStartPoint(std::chrono::system_clock::now());
        auto utahGraphLoader = UtahGraphLoader::get_instance();
        auto graph = utahGraphLoader.load();
        utahGraphLoader.show();
        graph.consolidate();

        auto dpso_config = DPSOConfig();
        auto solver = DPSO(graph, 6, 26, dpso_config);
        solver.solve();

        profiler.setStopPoint(std::chrono::system_clock::now());
        utahGraphLoader.show(solver.get_best_position());

        cout << "General algorithm time: " << profiler.getLastTotalDuration() << " [ms]" << endl;
        cout << "Critical loop time: " << profiler.getLastCriticalLoopDuration() << " [ms]" << endl;
    }

    cout << "\n<< Summary: >>" << endl;
    cout << "\t- avg general algorithm time: " << profiler.getAvgTotalDuration() << " [ms]" << endl;
    cout << "\t- avg critical loop time: " << profiler.getAvgCriticalLoopDuration() << " [ms]" << endl;

    profiler.saveToFile();

    return 0;
}
