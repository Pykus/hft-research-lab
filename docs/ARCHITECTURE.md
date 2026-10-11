# Architecture — initial stage

This is an educational research scaffold, **not** a live trading system.

- `Order`: immutable example order representation (integer price ticks; no floating-point prices).
- `is_valid`: basic field validation.
- `hft_demo`: tiny executable.
- `hft_tests`: first correctness checks.

Future milestones: limit order book, matching engine, replayable market data, simulated execution, risk controls, benchmark harness. Real L2 feeds do not reveal individual queue positions, so fill models must state their assumptions.
