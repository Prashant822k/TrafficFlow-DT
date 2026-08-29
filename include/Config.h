#pragma once
#include <array>
#include <string>

enum class Approach { North = 0, South = 1, East = 2, West = 3 };
inline const char* approach_name(Approach a) { static const char* n[] = {"north", "south", "east", "west"}; return n[static_cast<int>(a)]; }
struct SimulationConfig {
  std::string scenario{"normal"}; int duration{3600}; unsigned seed{42};
  std::array<double,4> arrival_rates{{0.18,0.18,0.18,0.18}};
  int fixed_green{30}; int yellow{3}; int min_green{10}; int max_green{45};
  int discharge_rate{1}; double queue_weight{1.0}; double waiting_weight{0.025};
};
inline SimulationConfig scenario_config(const std::string& name) {
  SimulationConfig c; c.scenario = name;
  if (name == "low") c.arrival_rates = {{0.08,0.08,0.08,0.08}};
  else if (name == "peak") c.arrival_rates = {{0.38,0.38,0.38,0.38}};
  else if (name == "asymmetric") c.arrival_rates = {{0.38,0.34,0.10,0.10}};
  else if (name != "normal") throw std::invalid_argument("Unknown scenario: " + name);
  return c;
}
