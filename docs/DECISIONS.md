# Architecture Decision Log

## 2026-10-11 — Start with C++20 and WSL2

**Decision:** Start with a small, testable C++20 project on Ubuntu under WSL2, built with CMake. Keep the repository portable and document recovery steps.

**Rationale:** Low setup friction on Windows, Linux toolchain familiarity, and reproducible incremental development.

**Limits:** Final low-latency benchmark conclusions require controlled experiments on native Linux and documented hardware. Docker/toolchain pinning is deferred.

## 2026-10-11 — Price representation

**Decision:** Represent prices as integer ticks, not floating-point currency values.

**Rationale:** Avoid binary floating-point rounding in order matching logic. Instrument tick size and price scale will be modeled later.
