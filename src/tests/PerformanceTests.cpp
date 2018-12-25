#include "PerformanceTests.h"
#include <cassert>
#include <iostream>
#include <fstream>

#include "../utility/logger.h"
#include "../graph/Graph.h"
#include "../io/UtahGraphLoader.h"
#include "../dpso/DPSO.h"
#include "../dpso/DPSOStrategyMPI.h"

using namespace std;

PerformanceTests::PerformanceTests()
        : profiler_(Profiler::instance()) {}

void PerformanceTests::synchronizationTest(uint16_t iterationCounter) const {
    runRepeatedDPSO(iterationCounter);
    ONLY_MASTER(
            cout << "\t- beta coefficient: " << profiler_.getSynchronizationPartRatio() << endl;
    );
}

void PerformanceTests::parallelTest(uint16_t iterationCounter) const {
    runRepeatedDPSO(iterationCounter);
}

void PerformanceTests::runRepeatedDPSO(uint16_t iterationCounter) const {
    for (uint16_t i = 1; i <= iterationCounter; ++i) {
        ONLY_MASTER(
                cout << "\n## ITERATION << " << i << "/" << iterationCounter << " >>\t progressing ..." << endl;
        );
        functionalDPSOTest();
        ONLY_MASTER(
                cout << "General algorithm time: " << profiler_.getLastTotalDuration() << " [ms]" << endl;
                cout << "Critical loop time: " << profiler_.getLastCriticalLoopDuration() << " [ms]" << endl;
        );
    }

    ONLY_MASTER(
            cout << "\n<< Summary: >>" << endl;
            cout << "\t- avg general algorithm time: " << profiler_.getAvgTotalDuration() << " [ms]" << endl;
            cout << "\t- avg critical loop time: " << profiler_.getAvgCriticalLoopDuration() << " [ms]" << endl;
            profiler_.saveToFile();
    );
}

void PerformanceTests::functionalDPSOTest() const {
#if defined(MODE_MPI)
    MPI_STATUS(MPI_Barrier(MPI_COMM_WORLD));
#endif
    ONLY_MASTER(
            profiler_.registerStartPoint();
    );
    auto &utahGraphLoader = UtahGraphLoader::instance();
    auto graph = utahGraphLoader.load();
    graph.consolidate();
    ONLY_MASTER(
            utahGraphLoader.show();
    );

    auto dpso_config = DPSOConfig();

#if !defined(MODE_SEQN) && !defined(MODE_OPEN_MP) && !defined(MODE_MPI)
    dpso_config.detailLogs = true;
#endif
#ifdef MODE_MPI
    auto solver = DPSOStrategyMPI(graph, 6, 26, dpso_config);
#else
    auto solver = DPSO(graph, 6, 26, dpso_config);
#endif

    solver.solve();
    ONLY_MASTER(
            profiler_.registerStopPoint();
            utahGraphLoader.show(solver.get_best_position());
    );
}