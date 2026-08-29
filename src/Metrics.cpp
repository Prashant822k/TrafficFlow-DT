#include "Metrics.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>
void MetricsCollector::add_completion(int wait) { ++summary_.completed; total_wait_ += wait; if (wait > summary_.max_wait) summary_.max_wait = wait; }
void MetricsCollector::record(int time, const std::array<int,4>& q, SignalPhase phase) {
 int total = q[0]+q[1]+q[2]+q[3]; total_queue_ += total; ++samples_; if (total > summary_.max_queue) summary_.max_queue = total;
 double avg = summary_.completed ? static_cast<double>(total_wait_) / summary_.completed : 0.0;
 records_.push_back({time, q, total, summary_.completed, avg, phase});
}
void MetricsCollector::finalize(int duration) { summary_.average_wait = summary_.completed ? static_cast<double>(total_wait_) / summary_.completed : 0.0; summary_.average_queue = samples_ ? static_cast<double>(total_queue_) / samples_ : 0.0; summary_.throughput = duration ? static_cast<double>(summary_.completed) * 3600.0 / duration : 0.0; }
void MetricsCollector::write_csv(const std::string& output) const {
 std::ofstream out(output); if (!out) throw std::runtime_error("Cannot write output: " + output);
 out << "time,north_queue,south_queue,east_queue,west_queue,total_queue,vehicles_completed,average_waiting_time,current_phase\n";
 out << std::fixed << std::setprecision(4);
 for (const auto& r : records_) out << r.time << ',' << r.queues[0] << ',' << r.queues[1] << ',' << r.queues[2] << ',' << r.queues[3] << ',' << r.total_queue << ',' << r.vehicles_completed << ',' << r.average_waiting << ',' << TrafficSignal::name(r.phase) << '\n';
}
