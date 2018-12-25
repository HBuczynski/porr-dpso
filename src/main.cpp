#include <iostream>
#include <sstream>
#include <mpi.h>

#include "utility/logger.h"
#include "utility/Profiler.h"
#include "tests/PerformanceTests.h"

using namespace std;


int main(int argc, char *argv[]) {
#ifdef MODE_MPI
    MPI_STATUS(MPI_Init(nullptr, nullptr));

    int comm_size;
    MPI_STATUS(MPI_Comm_size(MPI_COMM_WORLD, &comm_size));

    int rank;
    MPI_STATUS(MPI_Comm_rank(MPI_COMM_WORLD, &rank));

    {
        std::stringstream ss;
        ss << "Starting porr_dpso from process " << rank << " out of " << comm_size << std::endl;
        std::cout << ss.str() << std::flush;
    }

    MPI_STATUS(MPI_Barrier(MPI_COMM_WORLD));

    // TODO fix me
#define MODE_SEQN

#else
    std::cout << "<< porr-dpso >>" << std::endl;
#endif

    Profiler &profiler = Profiler::instance();
    PerformanceTests performanceTests;

#ifdef MODE_OPEN_MP
    if (argc < 3) {
        cout << "You have to write iteration counter and thread number as input arguments !!!" << endl;
        return 1;
    }
    profiler.setThreadNumber(static_cast<uint8_t >(atoi(argv[2])));
    profiler.setMode("PARALLEL");

    auto iterationCounter = static_cast<uint16_t >(atoi(argv[1]));
    performanceTests.parallelTest(iterationCounter);
#elif defined(MODE_SEQN)
    ONLY_MPI_MASTER(
            if (argc < 2) {
                cout << "You have to write test's iteration number as an input argument !!!" << endl;
                return 1;
            }
            profiler.setMode("SEQN");
    );
    auto iterationCounter = static_cast<uint16_t>(atoi(argv[1]));
    performanceTests.synchronizationTest(iterationCounter);
#else
    // suppress unused variable warnings
    (void) argc;
    (void) argv;
    (void) profiler;

    performanceTests.functionalDPSOTest();
#endif

#if defined(MODE_MPI)
    MPI_STATUS(MPI_Barrier(MPI_COMM_WORLD));

    std::stringstream ss;
    ss << "Finalizing porr_dpso from process " << rank << " out of " << comm_size << std::endl;
    std::cout << ss.str();

    MPI_Finalize();
#endif

    return 0;
}

