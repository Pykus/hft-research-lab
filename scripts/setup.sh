#!/usr/bin/env bash
set -euo pipefail
if ! command -v apt-get >/dev/null; then echo "This setup script requires Ubuntu/Debian."; exit 1; fi
sudo apt-get update
sudo apt-get install -y build-essential cmake git gdb
bash scripts/verify.sh
