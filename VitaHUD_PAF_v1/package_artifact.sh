#!/usr/bin/env bash
set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
PACKAGE_DIR="$BUILD_DIR/VitaHUD_PAF_v4_package"

rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR/ur0/tai"
mkdir -p "$PACKAGE_DIR/ur0/data/VitaHUD"
mkdir -p "$PACKAGE_DIR/docs"

if [ -f "$BUILD_DIR/vitahud_paf_v4.suprx" ]; then
  cp "$BUILD_DIR/vitahud_paf_v4.suprx" "$PACKAGE_DIR/ur0/tai/vitahud_paf_v4.suprx"
fi

if [ -f "$BUILD_DIR/vitahud_plugin.rco" ]; then
  cp "$BUILD_DIR/vitahud_plugin.rco" "$PACKAGE_DIR/ur0/data/VitaHUD/vitahud_plugin.rco"
fi

if [ -d "$PROJECT_ROOT/VitaHUD_Shell/RES_RCO" ]; then
  cp -R "$PROJECT_ROOT/VitaHUD_Shell/RES_RCO/"* "$PACKAGE_DIR/ur0/data/VitaHUD/" || true
fi

cat > "$PACKAGE_DIR/INSTALL.txt" <<'EOF'
VitaHUD PAF v4 package

Current status:
- Phase 4 real PAF wiring start.
- Compatibility mode still works in GitHub Actions.
- Real mode requires real paf.h headers/imports and real RCO compilation.

Target paths:
  ur0:tai/vitahud_paf_v4.suprx
  ur0:data/VitaHUD/vitahud_plugin.rco

Future tai config target:
  *main
  ur0:tai/vitahud_paf_v4.suprx
EOF

cat > "$PACKAGE_DIR/docs/STATUS.txt" <<'EOF'
VitaHUD PAF v4 status

Added:
- Real PAF build switch
- VITAHUD_USE_REAL_PAF option
- Real PAF include/library path variables
- Optional RCO compiler hook
- Real-mode g_corePlugin TODO guard

Still needed:
- Real PAF headers
- Real PAF import libraries/stubs
- Real shell plugin context assignment for g_corePlugin
- Compiled vitahud_plugin.rco
EOF

cd "$BUILD_DIR"
tar -czf VitaHUD_PAF_v4_package.tar.gz VitaHUD_PAF_v4_package
