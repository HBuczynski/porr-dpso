#include "PerformanceTests.h"
#include <cassert>
#include <iostream>
#include <fstream>

#include "../graph/Graph.h"
#include "../io/UtahGraphLoader.h"
#include "../dpso/DPSO.h"

using namespace std;

PerformanceTests::PerformanceTests()
    : profiler_(Profiler::getInstance())
{}

PerformanceTests::~PerformanceTests()
{}

void PerformanceTests::synchronizationTest(uint16_t iterationCounter) const
{
    for(uint16_t i=1; i<=iterationCounter; ++i)
    {
        cout << "\n## ITERATION << " << i << "/" << iterationCounter << " >>\t progressing ..." << endl;

        profiler_.setStartPoint(std::chrono::system_clock::now());
        auto utahGraphLoader = UtahGraphLoader::get_instance();
        auto graph = utahGraphLoader.load();
        utahGraphLoader.show();
        graph.consolidate();

        auto dpso_config = DPSOConfig();
        auto solver = DPSO(graph, 6, 26, dpso_config);
        solver.solve();

        profiler_.setStopPoint(std::chrono::system_clock::now());
        utahGraphLoader.show(solver.get_best_position());

        cout << "General algorithm time: " << profiler_.getLastTotalDuration() << " [ms]" << endl;
        cout << "Critical loop time: " << profiler_.getLastCriticalLoopDuration() << " [ms]" << endl;
    }

    cout << "\n<< Summary: >>" << endl;
    cout << "\t- avg general algorithm time: " << profiler_.getAvgTotalDuration() << " [ms]" << endl;
    cout << "\t- avg critical loop time: " << profiler_.getAvgCriticalLoopDuration() << " [ms]" << endl;
    cout << "\t- beta coefficient: " << profiler_.getSynchronizationPartRatio() << endl;

    profiler_.saveToFile();
}

void PerformanceTests::pararellTest(uint16_t iterationCounter) const
{
    for(uint16_t i=1; i<=iterationCounter; ++i)
    {
        cout << "\n## ITERATION << " << i << "/" << iterationCounter << " >>\t progressing ..." << endl;

        profiler_.setStartPoint(std::chrono::system_clock::now());
        auto utahGraphLoader = UtahGraphLoader::get_instance();
        auto graph = utahGraphLoader.load();
        utahGraphLoader.show();
        graph.consolidate();

        auto dpso_config = DPSOConfig();
        auto solver = DPSO(graph, 6, 26, dpso_config);
        solver.solve();

        profiler_.setStopPoint(std::chrono::system_clock::now());
        utahGraphLoader.show(solver.get_best_position());

        cout << "General algorithm time: " << profiler_.getLastTotalDuration() << " [ms]" << endl;
        cout << "Critical loop time: " << profiler_.getLastCriticalLoopDuration() << " [ms]" << endl;
    }

    cout << "\n<< Summary: >>" << endl;
    cout << "\t- avg general algorithm time: " << profiler_.getAvgTotalDuration() << " [ms]" << endl;
    cout << "\t- avg critical loop time: " << profiler_.getAvgCriticalLoopDuration() << " [ms]" << endl;

    profiler_.saveToFile();
}

void PerformanceTests::functionalDPSOTest() const
{
    profiler_.setStartPoint(std::chrono::system_clock::now());
    auto utahGraphLoader = UtahGraphLoader::get_instance();
    auto graph = utahGraphLoader.load();
    utahGraphLoader.show();
    graph.consolidate();

    auto dpso_config = DPSOConfig();
    dpso_config.detailLogs = true;
    auto solver = DPSO(graph, 6, 26, dpso_config);
    solver.solve();

    profiler_.setStopPoint(std::chrono::system_clock::now());
    utahGraphLoader.show(solver.get_best_position());
}