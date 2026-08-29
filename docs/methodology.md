# Methodology

Each experiment runs one four-approach intersection for 3,600 one-second steps. Bernoulli arrivals use independent per-approach rates and `std::mt19937`; controller pairs use identical scenario/seed inputs. A green approach discharges up to one queued vehicle per second. Yellow serves no vehicles.

The fixed baseline uses 30-second greens and 3-second yellows. Queue-Aware Adaptive Signal Control must honour a 10-second minimum green and a 45-second maximum green. At a decision point it compares `queue_weight * directional queue + waiting_weight * directional mean waiting`, selecting the direction with greater score. The maximum-green bound ensures each direction is eventually served; it is a simple, interpretable heuristic—not reinforcement learning.

The five paired seeds are 42, 123, 456, 789, and 1000. Reported standard deviations are across these seeds. Average queue is the time average of total queued vehicles; throughput is completed vehicles per simulated hour.
