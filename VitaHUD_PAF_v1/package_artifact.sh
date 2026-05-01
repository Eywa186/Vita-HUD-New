#!/usr/bin/env bash
set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
PACKAGE_DIR="$BUILD_DIR/VitaHUD_PAF_v3_package"

rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR/ur0/tai"
mkdir -p "$PACKAGE_DIR/ur0/data/VitaHUD"
mkdir -p "$PACKAGE_DIR/docs"

# Copy plugin.
if [ -f "$BUILD_DIR/vitahud_paf_v3.suprx" ]; then
  cp "$BUILD_DIR/vitahud_paf_v3.suprx" "$PACKAGE_DIR/ur0/tai/vitahud_paf_v3.suprx"
elif [ -f "$BUILD_DIR/libvitahud_paf_v3.suprx" ]; then
  cp "$BUILD_DIR/libvitahud_paf_v3.suprx" "$PACKAGE_DIR/ur0/tai/vitahud_paf_v3.suprx"
elif [ -f "$BUILD_DIR/libvitahud_paf_v2.suprx" ]; then
  cp "$BUILD_DIR/libvitahud_paf_v2.suprx" "$PACKAGE_DIR/ur0/tai/vitahud_paf_v3.suprx"
fi

# Stage RCO/XML resources for the real PAF wiring phase.
if [ -d "$PROJECT_ROOT/VitaHUD_Shell/RES_RCO" ]; then
  cp -R "$PROJECT_ROOT/VitaHUD_Shell/RES_RCO/"* "$PACKAGE_DIR/ur0/data/VitaHUD/" || true
fi

cat > "$PACKAGE_DIR/INSTALL.txt" <<'EOF'
VitaHUD PAF v3.1 packaged artifact

Current status:
- This is a packaged compile-test build.
- It still uses paf_compat.h for GitHub Actions compilation.
- It is not the final real PAF overlay yet.

Package contents:
  ur0:tai/vitahud_paf_v3.suprx
  ur0:data/VitaHUD/vitahud_plugin.xml
  ur0:data/VitaHUD/locale/vitahud_locale_en.xml

Future tai config target:
  *main
  ur0:tai/vitahud_paf_v3.suprx

Do not replace your stable old VitaHUD yet.
EOF

cat > "$PACKAGE_DIR/docs/STATUS.txt" <<'EOF'
VitaHUD PAF v3.1 status

Working:
- VitaSDK GitHub Actions build
- C++ shell project structure
- HUD/Menu source split
- Profile source split
- RES_RCO staging folder
- Packaged artifact output
- Clean SUPRX filename without lib prefix

Not final yet:
- Real PAF headers/imports
- Real RCO compilation
- Real visible overlay
- RAM/IP telemetry
EOF

cd "$BUILD_DIR"
rm -f VitaHUD_PAF_v3_package.zip
zip -r VitaHUD_PAF_v3_package.zip VitaHUD_PAF_v3_package
