#ifndef PORR_DPSO_PERFORMANCETESTS_H
#define PORR_DPSO_PERFORMANCETESTS_H

#include "../utility/Profiler.h"
#include <cstdint>

class PerformanceTests {
public:
    PerformanceTests();

    void synchronizationTest(uint16_t iterationCounter) const;

    void parallelTest(uint16_t iterationCounter) const;

    void functionalDPSOTest() const;

private:
    void runRepeatedDPSO(uint16_t iterationCounter) const;

    Profiler &profiler_;
};


#endif //PORR_DPSO_PERFORMANCETESTS_H
