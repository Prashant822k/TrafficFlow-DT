#include "AdaptiveSignalController.h"
#include "FixedTimeController.h"
#include "TrafficSimulator.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
static void usage() { std::cout << "Usage: traffic_sim --scenario low|normal|peak|asymmetric --controller fixed|adaptive --seed N --duration N --output file.csv\n"; }
int main(int argc, char** argv) {
 try { std::string scenario="normal", controller="fixed", output="simulation.csv"; unsigned seed=42; int duration=3600;
  for(int i=1;i<argc;i+=2) { if(std::string(argv[i])=="--help") { usage(); return 0; } if(i+1>=argc) throw std::invalid_argument("Missing value for "+std::string(argv[i])); std::string k=argv[i], v=argv[i+1]; if(k=="--scenario")scenario=v; else if(k=="--controller")controller=v; else if(k=="--seed")seed=static_cast<unsigned>(std::stoul(v)); else if(k=="--duration")duration=std::stoi(v); else if(k=="--output")output=v; else throw std::invalid_argument("Unknown argument: "+k); }
  SimulationConfig c=scenario_config(scenario); c.seed=seed; c.duration=duration; if(duration<=0) throw std::invalid_argument("duration must be positive"); std::unique_ptr<SignalController> signal;
  if(controller=="fixed") signal=std::make_unique<FixedTimeController>(c.fixed_green,c.yellow); else if(controller=="adaptive") signal=std::make_unique<AdaptiveSignalController>(c.min_green,c.max_green,c.yellow,c.queue_weight,c.waiting_weight); else throw std::invalid_argument("controller must be fixed or adaptive");
  Summary s=TrafficSimulator(c,std::move(signal)).run(output); std::cout << "scenario="<<scenario<<" controller="<<controller<<" seed="<<seed<<" generated="<<s.generated<<" completed="<<s.completed<<" avg_wait="<<s.average_wait<<" avg_queue="<<s.average_queue<<" max_queue="<<s.max_queue<<" throughput_per_hour="<<s.throughput<<" output="<<output<<'\n'; return 0;
 } catch(const std::exception& e) { std::cerr << "Error: " << e.what() << '\n'; usage(); return 1; }
}
