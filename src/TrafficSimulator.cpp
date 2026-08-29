#include "TrafficSimulator.h"
#include <numeric>
TrafficSimulator::TrafficSimulator(SimulationConfig config, std::unique_ptr<SignalController> controller)
 : config_(config), controller_(std::move(controller)), rng_(config.seed) {}
std::array<int,4> TrafficSimulator::queue_sizes() const { std::array<int,4> a{}; for(int i=0;i<4;++i) a[i]=static_cast<int>(queues_[i].size()); return a; }
std::array<double,4> TrafficSimulator::average_waits() const { std::array<double,4> a{}; for(int i=0;i<4;++i) { long long s=0; for(const auto& v:queues_[i]) s+=v.waiting_time; a[i]=queues_[i].empty()?0.0:static_cast<double>(s)/queues_[i].size(); } return a; }
void TrafficSimulator::generate(int time) { for(int i=0;i<4;++i) { std::bernoulli_distribution arrivals(config_.arrival_rates[i]); if(arrivals(rng_)) { queues_[i].emplace_back(next_id_++, static_cast<Approach>(i), time); metrics_.add_generated(); } } }
void TrafficSimulator::discharge(int time, SignalPhase phase) { bool ns = phase == SignalPhase::NorthSouthGreen; bool ew = phase == SignalPhase::EastWestGreen; for(int i=0;i<4;++i) if ((i<2 && ns) || (i>=2 && ew)) for(int n=0;n<config_.discharge_rate && !queues_[i].empty();++n) { Vehicle v=queues_[i].front(); queues_[i].pop_front(); v.complete(time); metrics_.add_completion(v.waiting_time); } }
void TrafficSimulator::add_waiting_time() { for(auto& lane:queues_) for(auto& v:lane) v.wait_one_second(); }
Summary TrafficSimulator::run(const std::string& output) { controller_->reset(); for(int t=0;t<config_.duration;++t) { generate(t); signal_.set_phase(controller_->update(queue_sizes(), average_waits())); discharge(t, signal_.phase()); add_waiting_time(); metrics_.record(t, queue_sizes(), signal_.phase()); } metrics_.finalize(config_.duration); metrics_.write_csv(output); return metrics_.summary(); }
