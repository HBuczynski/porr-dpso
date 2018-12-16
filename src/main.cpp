#include <cassert>
#include <iostream>
#include <fstream>

#include "graph/Graph.h"
#include "io/UtahGraphLoader.h"
#include "dpso/DPSO.h"
#include "utility/Profiler.h"
#include "tests/PerformanceTests.h"

using namespace std;

void performTest();

int main(int argc, char *argv[]) {

    std::cout << "<< porr-dpso >>" << std::endl;
    Profiler &profiler = Profiler::getInstance();
    PerformanceTests performanceTests;

#ifdef PARARELL
    if (argc < 3)
    {
        cout << "You have to write iteration counter and thread number as input arguments !!!" << endl;
        return 0;
    }
    uint16_t iterationCounter = static_cast<uint16_t >(atoi(argv[1]));

    profiler.setThreadNumber(static_cast<uint8_t >(atoi(argv[2])));
    profiler.setMode("PARARELL");

    performanceTests.pararellTest(iterationCounter);
#elif SYNCH
    if (argc < 2)
    {
        cout << "You have to write test's iteration number as an input argument !!!" << endl;
        return 0;
    }
    uint16_t iterationCounter = static_cast<uint16_t >(atoi(argv[1]));

    profiler.setMode("SYNCH");
    performanceTests.synchronizationTest(iterationCounter);
#else
    performanceTests.functionalDPSOTest();
#endif

    return 0;
}

