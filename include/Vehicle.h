#pragma once
#include "Config.h"
struct Vehicle {
  int id; Approach approach; int arrival_time; int waiting_time{0}; int completion_time{-1};
  bool completed{false};
  Vehicle(int id_, Approach a, int t) : id(id_), approach(a), arrival_time(t) {}
  void wait_one_second() { if (!completed) ++waiting_time; }
  void complete(int time) { completed = true; completion_time = time; }
};
