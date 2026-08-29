#pragma once
#include "Config.h"
#include "Metrics.h"
#include "SignalController.h"
#include "TrafficSignal.h"
#include "Vehicle.h"
#include <array>
#include <deque>
#include <memory>
#include <random>
class TrafficSimulator {
 public:
  TrafficSimulator(SimulationConfig config, std::unique_ptr<SignalController> controller);
  Summary run(const std::string& output);
 private:
  std::array<int,4> queue_sizes() const; std::array<double,4> average_waits() const;
  void generate(int time); void discharge(int time, SignalPhase phase); void add_waiting_time();
  SimulationConfig config_; std::unique_ptr<SignalController> controller_; TrafficSignal signal_; MetricsCollector metrics_;
  std::array<std::deque<Vehicle>,4> queues_; std::mt19937 rng_; int next_id_{1};
};
