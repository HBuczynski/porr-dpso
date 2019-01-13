#include "Profiler.h"

#include <cassert>
#include <numeric>
#include <functional>
#include <iostream>
#include <fstream>

using namespace std;
using namespace chrono;

Profiler *Profiler::instance_ = nullptr;
mutex Profiler::mutex_;

Profiler::Profiler()
        : threadNumber_(1) {}

Profiler &Profiler::getInstance() {
    if (!instance_) {
        lock_guard<mutex> lock(mutex_);
        if (!instance_) {
            instance_ = new Profiler();
        }
    }
    return *instance_;
}

void Profiler::registerStartPoint() {
    startTimePoint_.push_back(system_clock::now());
}

void Profiler::registerParallelisationStartPoint() {
    startParallelisationTimePoint_.push_back(system_clock::now());
}

void Profiler::registerParallelisationStopPoint() {
    stopParallelisationTimePoint_.push_back(system_clock::now());
}

void Profiler::registerStopPoint() {
    stopTimePoint_.push_back(system_clock::now());
}

void Profiler::setThreadNumber(uint8_t number) {
    threadNumber_ = number;
}

void Profiler::setMode(const string &mode) {
    mode_ = mode;
}

uint8_t Profiler::getThreadNumber() const {
    return threadNumber_;
}

double Profiler::getLastTotalDuration() const {
    assert(startTimePoint_.size() == stopTimePoint_.size() && "Vectors have a different size.");
    if (stopTimePoint_.empty())
        return 0.0;
    return duration_cast<milliseconds>(stopTimePoint_.back() - startTimePoint_.back()).count();
}

double Profiler::getLastCriticalLoopDuration() const {
    assert(stopParallelisationTimePoint_.size() == stopParallelisationTimePoint_.size() &&
           "Vectors have a different size.");
    if (stopParallelisationTimePoint_.empty())
        return 0.0;
    return duration_cast<milliseconds>(
            stopParallelisationTimePoint_.back() - startParallelisationTimePoint_.back()).count();
}

double Profiler::getAvgTotalDuration() const {
    assert(startTimePoint_.size() == stopTimePoint_.size() && "Vectors have a different size.");
    if (startTimePoint_.empty())
        return 0.0;

    vector<uint32_t> durations(startTimePoint_.size());
    transform(startTimePoint_.begin(), startTimePoint_.end(), stopTimePoint_.begin(), durations.begin(),
              [](const auto &x1_ele, const auto &x2_ele) {
                  return duration_cast<milliseconds>(x2_ele - x1_ele).count();
              });

    return accumulate(durations.begin(), durations.end(), 0) / durations.size();
}

double Profiler::getAvgCriticalLoopDuration() const {
    assert(startParallelisationTimePoint_.size() == stopParallelisationTimePoint_.size() &&
           "Vectors have a different size.");
    if (startParallelisationTimePoint_.empty())
        return 0.0;

    vector<uint32_t> durations(startParallelisationTimePoint_.size());
    transform(startParallelisationTimePoint_.begin(), startParallelisationTimePoint_.end(),
              stopParallelisationTimePoint_.begin(), durations.begin(),
              [](const auto &x1_ele, const auto &x2_ele) {
                  return duration_cast<milliseconds>(x2_ele - x1_ele).count();
              });

    return accumulate(durations.begin(), durations.end(), 0) / durations.size();
}

double Profiler::getSynchronizationPartRatio() const {
    const auto avgCriticalLoop = getAvgCriticalLoopDuration();
    const auto avgTotalDuration = getAvgTotalDuration();

    return (avgTotalDuration - avgCriticalLoop) / avgTotalDuration;
}

double Profiler::getSpeedUpCoefficientEstimation() const {
    const auto betaCoefficient = getSynchronizationPartRatio();

    return 1 / (betaCoefficient + (1 - betaCoefficient) / threadNumber_);
}

void Profiler::saveToFile() {
    ofstream file(string("output/") + mode_ + string(".txt"), std::fstream::out);
    if (!file.is_open()) {
        cout << "\n\nCannot open output file\n\n";
        return;
    }

    file << "general_duration [ms]\t" << "loop_duration [ms]\n";
    for (uint16_t i = 0; i < startTimePoint_.size(); ++i) {
        file << duration_cast<milliseconds>(stopTimePoint_[i] - startTimePoint_[i]).count();
        file << "\t";
        double diff = 0.0;
        if (!stopParallelisationTimePoint_.empty())
            diff = duration_cast<milliseconds>(
                    stopParallelisationTimePoint_[i] - startParallelisationTimePoint_[i]).count();
        file << diff;
        file << "\n";
    }

    file.close();

    cout << "\nSaved samples to file." << endl;
}