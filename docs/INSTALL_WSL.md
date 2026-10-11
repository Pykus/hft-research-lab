# Install and reproduce on WSL2

1. In Windows PowerShell: `wsl --install -d Ubuntu` (restart if requested).
2. Open Ubuntu and create your Linux user.
3. In Ubuntu:

```bash
sudo apt update && sudo apt install -y git
cd ~
git clone https://github.com/Pykus/hft-research-lab.git
cd hft-research-lab
bash scripts/setup.sh
```

Expected: CMake build succeeds, CTest passes, demo prints an example BUY order.

To rerun: `bash scripts/verify.sh`.

Keep the repository in the Linux filesystem (e.g. `~/hft-research-lab`) rather than `/mnt/c` for typical WSL development.

## Export / import WSL (PowerShell)

Stop processes and run `wsl --shutdown`, then:

```powershell
wsl --export Ubuntu hft-lab-backup.tar
wsl --import HFT-Lab C:\\WSL\\HFT-Lab .\\hft-lab-backup.tar --version 2
```

An imported distribution may start as root; configure its default user separately. WSL2 must be installed on the destination computer. Never commit backups or secrets to Git.

## Reproducibility

Record `uname -a`, `g++ --version`, `cmake --version`, CPU model, build type, git commit, and benchmark input data. WSL/virtualized latency measurements are not equivalent to native Linux measurements. Package versions from apt can change over time; a later milestone will lock toolchain/container versions.
