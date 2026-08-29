#include "TrafficSignal.h"
std::string TrafficSignal::name(SignalPhase p) {
 switch (p) { case SignalPhase::NorthSouthGreen: return "NORTH_SOUTH_GREEN"; case SignalPhase::NorthSouthYellow: return "NORTH_SOUTH_YELLOW"; case SignalPhase::EastWestGreen: return "EAST_WEST_GREEN"; case SignalPhase::EastWestYellow: return "EAST_WEST_YELLOW"; } return "UNKNOWN";
}
