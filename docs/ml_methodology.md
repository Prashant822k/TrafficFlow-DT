# ML Methodology

The small ML exercise predicts total queue at `t + 1` from observations available at `t`: four approach queues, total queue, cumulative completions, mean completed-vehicle waiting time, and current signal phase. `train_model.py` shifts the target by one row and drops the final row from every run. It never uses a future state feature, avoiding target leakage.

The model is ordinary linear regression with a deterministic random 80/20 holdout split. Its purpose is to demonstrate an explainable tabular ML pipeline, not to control the intersection or claim real-world forecasting validity. In a future mobility Digital Twin, synthetic observations could be replaced or augmented by sensor, GPS, signal-state, and road-network data.
