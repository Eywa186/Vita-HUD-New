#!/usr/bin/env bash
set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
PACKAGE_DIR="$BUILD_DIR/VitaHUD_PAF_v3_package"

rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR/ur0/tai"
mkdir -p "$PACKAGE_DIR/ur0/data/VitaHUD"
mkdir -p "$PACKAGE_DIR/docs"

# Copy plugin if it exists.
if [ -f "$BUILD_DIR/vitahud_paf_v2.suprx" ]; then
  cp "$BUILD_DIR/vitahud_paf_v2.suprx" "$PACKAGE_DIR/ur0/tai/vitahud_paf_v2.suprx"
fi

# Stage RCO/XML resources for the real PAF wiring phase.
if [ -d "$PROJECT_ROOT/VitaHUD_Shell/RES_RCO" ]; then
  cp -R "$PROJECT_ROOT/VitaHUD_Shell/RES_RCO/"* "$PACKAGE_DIR/ur0/data/VitaHUD/" || true
fi

cat > "$PACKAGE_DIR/INSTALL.txt" <<'EOF'
VitaHUD PAF v3 packaged artifact

Current status:
- This is a packaged compile-test build.
- It still uses paf_compat.h for GitHub Actions compilation.
- It is not the final real PAF overlay yet.

Install path target later:
  ur0:tai/vitahud_paf_v2.suprx
  ur0:data/VitaHUD/vitahud_plugin.rco

For this staged build, RES_RCO XML files are included as preparation only.

Example tai config target later:
  *main
  ur0:tai/vitahud_paf_v2.suprx

Do not replace your stable old VitaHUD yet.
EOF

cat > "$PACKAGE_DIR/docs/STATUS.txt" <<'EOF'
VitaHUD PAF v3 status

Working:
- VitaSDK GitHub Actions build
- C++ shell project structure
- HUD/Menu source split
- Profile source split
- RES_RCO staging folder
- Packaged artifact output

Not final yet:
- Real PAF headers/imports
- Real RCO compilation
- Real visible overlay
- RAM/IP telemetry
EOF

cd "$BUILD_DIR"
zip -r VitaHUD_PAF_v3_package.zip VitaHUD_PAF_v3_package
