# VitaHUD PAF v1.1 Skeleton

Phase 1 goal:

- Move VitaHUD away from direct framebuffer drawing.
- Use a PAF/shell-style overlay structure.
- Start with a clean HUD + menu architecture.
- No RAM/IP yet.
- No framebuffer drawing.
- No copied PSVshellPlus files.

This is a fresh VitaHUD skeleton that uses the same kind of design idea:
a shell overlay page + main-thread UI updates + cached text values.

## Current target features

- L + R + START opens/closes menu
- FPS / battery / time HUD placeholders
- Black configurable menu panel
- Profiles/hooks prepared
- CPU/BUS/GPU placeholders prepared
- RAM/IP intentionally disabled for Phase 1

## Important

This is a starting architecture/skeleton, not the final finished plugin.

PAF projects require the Vita shell/plugin environment, PAF headers, and correct linking setup.
Depending on your SDK setup, the CMake file may need adjustment.

## Files

```txt
VitaHUD_Shell/main.cpp
VitaHUD_Shell/global.h
VitaHUD_Shell/hud.cpp
VitaHUD_Shell/hud.h
VitaHUD_Shell/menu.cpp
VitaHUD_Shell/menu.h
VitaHUD_Shell/profile.cpp
VitaHUD_Shell/profile.h
```

## Phase 2

After this skeleton builds/loads, port over:

- profile save/load
- colors/themes
- HUD position
- font/size options
- CPU/BUS/GPU
- APP ID

## Phase 3

Add RAM through a kernel-side helper instead of direct userland framebuffer/plugin calls.


## v1.1

Fixed VitaSDK header paths such as `kernel.h` -> `psp2/kernel/threadmgr.h`.


## v1.2

Fixed invalid header `psp2/kernel/types.h` -> `psp2/types.h`.

## v1.3

Added `VitaHUD_Shell/paf_compat.h`.

This is a temporary compile shim for GitHub Actions because the normal VitaSDK
container does not include real `paf.h`.

This version is for build-progress only. It does not create a real visible PAF
overlay until real PAF headers/RCO wiring are added.

## v1.4

Fixed module macros for the GitHub VitaSDK container.

Changed:
- `SCE_MODULE_INFO(...)` -> `PSP2_MODULE_INFO(...)`
- removed unsupported `SCE_MODULE_START(...)`
- removed unsupported `SCE_MODULE_STOP(...)`

## v1.5

Removed module-info macro entirely for the first compile pass.

The GitHub VitaSDK container did not define:
- `SCE_MODULE_INFO`
- `PSP2_MODULE_INFO`

For now we only need the skeleton compiling. Real module metadata/export setup can be added after
the PAF structure compiles.
