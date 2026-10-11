# Research journal

## 2026-10-11 — Project initialization

**Goal:** Gradually develop an experimental C++20 market microstructure and low-latency trading lab. Potential research directions include order book data structures, cache behavior, and tail latency.

**Stage 0:** Repository, reproducible WSL setup, basic order model, build and tests.

**Next:** Install WSL2 on a development machine, clone this repository, run `bash scripts/setup.sh`, and record toolchain/hardware details. Scientific hypothesis and evaluation protocol are not yet selected.

**Important:** This repository is the canonical technical log; any Google Drive backup must be separately configured and verified.

## 2026-10-11 — Stage 0 confirmed by developer

**Reported result:** Ubuntu on WSL installed, project cloned, setup completed and tests passed.

**Verification level:** Developer-reported; no direct inspection of this WSL instance or captured compiler/OS versions yet.

**Next lesson:** Understand the `Order` structure, integer tick prices, `enum class Side`, and basic validation. Then introduce a small, tested change.

**Next reproducibility checkpoint:** Record `lsb_release -a`, `g++ --version`, `cmake --version`, `uname -a`, and `git rev-parse HEAD` from the development WSL instance.
