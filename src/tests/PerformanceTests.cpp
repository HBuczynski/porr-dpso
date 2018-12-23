#include "PerformanceTests.h"
#include <cassert>
#include <iostream>
#include <fstream>

#include "../graph/Graph.h"
#include "../io/UtahGraphLoader.h"
#include "../dpso/DPSO.h"

using namespace std;

PerformanceTests::PerformanceTests()
        : profiler_(Profiler::getInstance()) {}

void PerformanceTests::synchronizationTest(uint16_t iterationCounter) const {
    runRepeatedDPSO(iterationCounter);
    cout << "\t- beta coefficient: " << profiler_.getSynchronizationPartRatio() << endl;
}

void PerformanceTests::parallelTest(uint16_t iterationCounter) const {
    runRepeatedDPSO(iterationCounter);
}

void PerformanceTests::runRepeatedDPSO(uint16_t iterationCounter) const {
    for (uint16_t i = 1; i <= iterationCounter; ++i) {
        cout << "\n## ITERATION << " << i << "/" << iterationCounter << " >>\t progressing ..." << endl;
        functionalDPSOTest();
        cout << "General algorithm time: " << profiler_.getLastTotalDuration() << " [ms]" << endl;
        cout << "Critical loop time: " << profiler_.getLastCriticalLoopDuration() << " [ms]" << endl;
    }

    cout << "\n<< Summary: >>" << endl;
    cout << "\t- avg general algorithm time: " << profiler_.getAvgTotalDuration() << " [ms]" << endl;
    cout << "\t- avg critical loop time: " << profiler_.getAvgCriticalLoopDuration() << " [ms]" << endl;

    profiler_.saveToFile();
}

void PerformanceTests::functionalDPSOTest() const {
    profiler_.registerStartPoint();
    auto utahGraphLoader = UtahGraphLoader::get_instance();
    auto graph = utahGraphLoader.load();
    utahGraphLoader.show();
    graph.consolidate();

    auto dpso_config = DPSOConfig();

#if !defined(MODE_SEQN) && !defined(MODE_PARALLEL)
    dpso_config.detailLogs = true;
#endif
    auto solver = DPSO(graph, 6, 26, dpso_config);
    solver.solve();

    profiler_.registerStopPoint();
    utahGraphLoader.show(solver.get_best_position());
}