#ifndef PORR_DPSO_PROFILER_H
#define PORR_DPSO_PROFILER_H

#include <vector>
#include <chrono>
#include <string>
#include <mutex>

using TimeContainer = std::vector<std::chrono::system_clock::time_point>;
using TimePoint = std::chrono::system_clock::time_point;

class Profiler
{
public:
    static Profiler &getInstance();

    void setStartPoint(const TimePoint& timePoint);
    void setParallelisationStartPoint(const TimePoint& timePoint);
    void setParallelisationStopPoint(const TimePoint& timePoint);
    void setStopPoint(const TimePoint& timePoint);

    void setThreadNumber(uint8_t number);
    void setMode(const std::string& mode);
    uint8_t getThreadNumber() const;

    double getLastTotalDuration() const;
    double getLastCriticalLoopDuration() const;

    double getAvgTotalDuration() const;
    double getAvgCriticalLoopDuration() const;

    void saveToFile();

private:
    explicit Profiler();
    ~Profiler();

    static Profiler *instance_;
    static std::mutex mutex_;

    uint8_t threadNumber_;
    std::string mode_;

    TimeContainer startTimePoint_;
    TimeContainer stopTimePoint_;
    TimeContainer startParallelisationTimePoint_;
    TimeContainer stopParallelisationTimePoint_;
};


#endif
