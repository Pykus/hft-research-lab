#!/usr/bin/env bash
set -euo pipefail
for tool in g++ cmake git; do command -v "$tool" >/dev/null || { echo "Missing: $tool"; exit 1; }; done
echo "Compiler: $(g++ --version | head -n 1)"
echo "CMake: $(cmake --version | head -n 1)"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j 2
ctest --test-dir build --output-on-failure
./build/hft_demo
