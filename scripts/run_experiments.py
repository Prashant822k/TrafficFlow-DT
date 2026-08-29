"""Run the fixed/adaptive comparison with matched seeds and create results/summary.csv."""
from __future__ import annotations
import argparse, csv, re, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SUMMARY_PATTERN = re.compile(r"(\w+)=([^ ]+)")

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", default=str(ROOT / "build" / "traffic_sim"))
    parser.add_argument("--duration", type=int, default=3600)
    args = parser.parse_args()
    binary = Path(args.binary)
    if not binary.exists() and binary.with_suffix(".exe").exists(): binary = binary.with_suffix(".exe")
    output_dir = ROOT / "results"; output_dir.mkdir(exist_ok=True)
    rows = []
    for scenario in ("low", "normal", "peak", "asymmetric"):
        for controller in ("fixed", "adaptive"):
            for seed in (42, 123, 456, 789, 1000):
                output = output_dir / f"{scenario}_{controller}_{seed}.csv"
                command = [str(binary), "--scenario", scenario, "--controller", controller,
                           "--seed", str(seed), "--duration", str(args.duration), "--output", str(output)]
                completed = subprocess.run(command, check=True, text=True, capture_output=True)
                values = dict(SUMMARY_PATTERN.findall(completed.stdout))
                rows.append({"scenario": scenario, "controller": controller, "seed": seed,
                             "generated": values["generated"], "completed": values["completed"],
                             "average_wait": values["avg_wait"], "average_queue": values["avg_queue"],
                             "max_queue": values["max_queue"], "throughput_per_hour": values["throughput_per_hour"],
                             "timeseries_csv": output.name})
    with (output_dir / "summary.csv").open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=rows[0].keys()); writer.writeheader(); writer.writerows(rows)
    print(f"Wrote {len(rows)} matched runs to {output_dir / 'summary.csv'}")

if __name__ == "__main__": main()
