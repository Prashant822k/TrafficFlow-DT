"""Aggregate simulator experiment outputs. No metrics are hard-coded."""
from pathlib import Path
import pandas as pd
ROOT = Path(__file__).resolve().parents[1]
df = pd.read_csv(ROOT / "results" / "summary.csv")
metrics = ["average_wait", "average_queue", "max_queue", "throughput_per_hour"]
summary = df.groupby(["scenario", "controller"])[metrics].agg(["mean", "std"])
summary.columns = ["_".join(column) for column in summary.columns]
summary = summary.reset_index()
fixed = summary[summary.controller == "fixed"].set_index("scenario")
adaptive = summary[summary.controller == "adaptive"].set_index("scenario")
improvement = pd.DataFrame({"scenario": fixed.index,
    "wait_improvement_pct": 100 * (fixed.average_wait_mean - adaptive.average_wait_mean) / fixed.average_wait_mean,
    "queue_improvement_pct": 100 * (fixed.average_queue_mean - adaptive.average_queue_mean) / fixed.average_queue_mean}).reset_index(drop=True)
print("\nController summary (mean and sample standard deviation across seeds):")
print(summary.to_string(index=False, float_format=lambda x: f"{x:.3f}"))
print("\nAdaptive improvement relative to fixed (%):")
print(improvement.to_string(index=False, float_format=lambda x: f"{x:.2f}"))
summary.to_csv(ROOT / "results" / "aggregate_summary.csv", index=False)
improvement.to_csv(ROOT / "results" / "improvements.csv", index=False)
