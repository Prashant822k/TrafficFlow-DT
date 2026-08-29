from pathlib import Path
import matplotlib.pyplot as plt
import pandas as pd
ROOT = Path(__file__).resolve().parents[1]; RESULTS = ROOT / "results"; FIGURES = RESULTS / "figures"; FIGURES.mkdir(exist_ok=True)
summary = pd.read_csv(RESULTS / "summary.csv")
# Matched seed 42 time series makes phase-by-phase behavior easy to inspect.
fig, axes = plt.subplots(2, 2, figsize=(11, 7), sharex=True)
for ax, scenario in zip(axes.flat, ["low", "normal", "peak", "asymmetric"]):
    for controller in ("fixed", "adaptive"):
        df = pd.read_csv(RESULTS / f"{scenario}_{controller}_42.csv")
        ax.plot(df.time, df.total_queue, label=controller)
    ax.set_title(scenario.capitalize()); ax.set_ylabel("Total queue"); ax.legend()
for ax in axes[-1]: ax.set_xlabel("Time (s)")
fig.tight_layout(); fig.savefig(FIGURES / "queue_over_time.png", dpi=160); plt.close(fig)
means = summary.groupby(["scenario", "controller"])[["average_wait", "throughput_per_hour"]].mean().reset_index()
for metric, label, filename in [("average_wait", "Average waiting time (s)", "waiting_comparison.png"), ("throughput_per_hour", "Throughput (vehicles/hour)", "throughput_comparison.png")]:
    pivot = means.pivot(index="scenario", columns="controller", values=metric).reindex(["low","normal","peak","asymmetric"])
    ax = pivot.plot.bar(rot=0, ylabel=label, title=label + " by scenario", figsize=(8,4))
    ax.figure.tight_layout(); ax.figure.savefig(FIGURES / filename, dpi=160); plt.close(ax.figure)
print(f"Saved figures in {FIGURES}")
