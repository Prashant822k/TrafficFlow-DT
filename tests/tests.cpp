#include "AdaptiveSignalController.h"
#include "FixedTimeController.h"
#include "TrafficSimulator.h"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
int main() {
 FixedTimeController fixed(2, 1); assert(fixed.update({}, {}) == SignalPhase::NorthSouthGreen); assert(fixed.update({}, {}) == SignalPhase::NorthSouthYellow); assert(fixed.update({}, {}) == SignalPhase::EastWestGreen);
 AdaptiveSignalController adaptive(1, 10, 1, 1.0, 0.0); std::array<int,4> ew{{0,0,10,10}}; assert(adaptive.update(ew,{}) == SignalPhase::NorthSouthYellow);
 SimulationConfig c=scenario_config("low"); c.duration=30; c.seed=42; c.arrival_rates={{1,0,0,0}}; c.fixed_green=5; std::unique_ptr<SignalController> controller=std::make_unique<FixedTimeController>(c.fixed_green,c.yellow); Summary s=TrafficSimulator(c,std::move(controller)).run("test_output.csv"); assert(s.generated==30); assert(s.completed>0); assert(s.average_wait>=0); std::ifstream f("test_output.csv"); assert(f.good()); f.close(); std::remove("test_output.csv");
 SimulationConfig red=scenario_config("low"); red.duration=1; red.arrival_rates={{0,0,1,0}}; red.fixed_green=5; Summary red_summary=TrafficSimulator(red,std::make_unique<FixedTimeController>(5,3)).run("red.csv"); assert(red_summary.generated==1 && red_summary.completed==0 && red_summary.average_queue==1); std::remove("red.csv");
 SimulationConfig repeat=scenario_config("normal"); repeat.duration=50; repeat.seed=123; Summary one=TrafficSimulator(repeat,std::make_unique<FixedTimeController>(repeat.fixed_green,repeat.yellow)).run("one.csv"); Summary two=TrafficSimulator(repeat,std::make_unique<FixedTimeController>(repeat.fixed_green,repeat.yellow)).run("two.csv"); assert(one.generated==two.generated && one.completed==two.completed && one.max_queue==two.max_queue && one.average_wait==two.average_wait); std::remove("one.csv"); std::remove("two.csv");
 std::cout << "All trafficflow-dt tests passed.\n";
}
