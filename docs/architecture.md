# Architecture

```text
Observed / synthetic traffic
          |
          v
 C++ discrete-time intersection model ---> CSV observations ---> Python analysis / ML
          |                                      |                    |
          v                                      v                    v
 SignalController interface                 experiment metrics   queue(t+1) prediction
    |                 |
FixedTimeController  AdaptiveSignalController
```

`TrafficSimulator` owns queues, random arrival generation, simulation sequencing, and signal state. It only calls the `SignalController` interface, which keeps controller policy separate from simulation mechanics. `MetricsCollector` owns time-series recording and CSV output.

This is a simulation-oriented Digital Twin prototype: it provides a digital representation, scenario evaluation, and a path for observed data. It is not a production Digital Twin deployment.
