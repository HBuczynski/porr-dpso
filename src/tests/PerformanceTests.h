#ifndef PORR_DPSO_PERFORMANCETESTS_H
#define PORR_DPSO_PERFORMANCETESTS_H

#include "../utility/Profiler.h"
#include <cstdint>

class PerformanceTests
{
public:
    PerformanceTests();
    ~PerformanceTests();

    void synchronizationTest(uint16_t iterationCounter) const;
    void pararellTest(uint16_t iterationCounter) const;
    void functionalDPSOTest() const;

private:
    Profiler &profiler_;
};


#endif //PORR_DPSO_PERFORMANCETESTS_H
