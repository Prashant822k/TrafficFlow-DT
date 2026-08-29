"""Predict queue(t+1) using only fields observable at time t (no future leakage)."""
from pathlib import Path
import joblib, numpy as np, pandas as pd
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
from sklearn.model_selection import train_test_split
ROOT = Path(__file__).resolve().parents[1]
frames = []
for path in sorted((ROOT / "results").glob("*_*.csv")):
    if path.name in {"summary.csv", "aggregate_summary.csv", "improvements.csv"}: continue
    df = pd.read_csv(path)
    if "total_queue" not in df: continue
    df["next_total_queue"] = df.total_queue.shift(-1); frames.append(df.iloc[:-1])
data = pd.concat(frames, ignore_index=True)
phase = pd.get_dummies(data.current_phase, prefix="phase")
features = pd.concat([data[["total_queue", "north_queue", "south_queue", "east_queue", "west_queue", "vehicles_completed", "average_waiting_time"]], phase], axis=1)
x_train, x_test, y_train, y_test = train_test_split(features, data.next_total_queue, test_size=0.2, random_state=42)
model = LinearRegression().fit(x_train, y_train); pred = model.predict(x_test)
metrics = {"MAE": mean_absolute_error(y_test,pred), "RMSE": mean_squared_error(y_test,pred)**0.5, "R2": r2_score(y_test,pred)}
print("Queue(t+1) linear-regression evaluation (random 80/20 split):")
for name, value in metrics.items(): print(f"{name}: {value:.4f}")
joblib.dump({"model": model, "feature_columns": list(features.columns)}, ROOT / "results" / "queue_predictor.joblib")
