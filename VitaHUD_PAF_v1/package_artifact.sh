#!/usr/bin/env bash
set -euo pipefail
mkdir -p artifact/ur0/tai artifact/ur0/data/VitaHUD
cp build/vitahud_paf_v5.suprx artifact/ur0/tai/vitahud_paf_v5.suprx
cp README_INSTALL.txt artifact/README_INSTALL.txt
tar -czf VitaHUD_PAF_v6_6_hard_alive_log_package.tar.gz -C artifact .
