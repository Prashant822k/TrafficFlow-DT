#pragma once
#include "TrafficSignal.h"
#include <array>
#include <string>
#include <vector>
struct TimeStepRecord { int time; std::array<int,4> queues; int total_queue; int vehicles_completed; double average_waiting; SignalPhase phase; };
struct Summary { int generated{0}; int completed{0}; double average_wait{0}; int max_wait{0}; double average_queue{0}; int max_queue{0}; double throughput{0}; };
class MetricsCollector {
 public:
  void add_generated() { ++summary_.generated; }
  void add_completion(int wait);
  void record(int time, const std::array<int,4>& q, SignalPhase phase);
  const Summary& summary() const { return summary_; }
  void finalize(int duration);
  void write_csv(const std::string& output) const;
 private: Summary summary_; long long total_wait_{0}; long long total_queue_{0}; int samples_{0}; std::vector<TimeStepRecord> records_;
};
