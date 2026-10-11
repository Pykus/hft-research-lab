# HFT Research Lab

A reproducible, incremental **C++20** research project for market microstructure, order books, matching engines, and low-latency engineering. **Educational/research only; no real orders or trading connectivity.**

## Quick start (Ubuntu / WSL2)

```bash
sudo apt update && sudo apt install -y git
git clone https://github.com/Pykus/hft-research-lab.git
cd hft-research-lab
bash scripts/setup.sh
```

Or, if dependencies are already installed:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j 2
ctest --test-dir build --output-on-failure
./build/hft_demo
```

## Documentation

- [WSL installation and recovery](docs/INSTALL_WSL.md)
- [Architecture](docs/ARCHITECTURE.md)
- [Decision log](docs/DECISIONS.md)
- [Research journal](docs/RESEARCH_LOG.md)
- [Benchmark methodology](docs/BENCHMARKS.md)

## Roadmap

1. Reproducible setup, order representation, tests.
2. Order book and matching rules.
3. Market data replay and deterministic simulation.
4. Benchmarking and profiling.
5. Research hypotheses and controlled comparisons.

The project intentionally starts small; features and research claims must be validated step by step.

## Learn C++20 with this repository

Follow the [HFT C++20 Complete Course](Complete-Course/README.md) for concise English lessons and practical exercises based on the actual project code. Start with [Lesson 01: Orders and Debugging](Complete-Course/lesson-01/README.md).
