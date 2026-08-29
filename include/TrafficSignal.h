#pragma once
#include <string>
enum class SignalPhase { NorthSouthGreen, NorthSouthYellow, EastWestGreen, EastWestYellow };
class TrafficSignal {
 public:
  explicit TrafficSignal(SignalPhase phase = SignalPhase::NorthSouthGreen) : phase_(phase) {}
  SignalPhase phase() const { return phase_; }
  void set_phase(SignalPhase phase) { phase_ = phase; }
  bool is_green_for_ns() const { return phase_ == SignalPhase::NorthSouthGreen; }
  bool is_green_for_ew() const { return phase_ == SignalPhase::EastWestGreen; }
  static std::string name(SignalPhase phase);
 private: SignalPhase phase_;
};
