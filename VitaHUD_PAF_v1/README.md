# VitaHUD PAF v4 Real PAF Start

This is Phase 4: real PAF/RCO wiring begins.

## What changed from v3

v3 was only a compile/package pipeline using `paf_compat.h`.

v4 adds the real-mode switch:

```bash
-DVITAHUD_USE_REAL_PAF=ON
```

When real mode is ON, `global.h` uses:

```cpp
#include <paf.h>
```

When real mode is OFF, GitHub Actions still compiles using:

```cpp
#include "paf_compat.h"
```

## Why this matters

The normal VitaSDK GitHub container does not include real PAF headers/imports.
So v4 supports both:

```txt
Compatibility build = keeps GitHub passing
Real PAF build      = used when real PAF headers/imports are available
```

## Build options

Compatibility mode:

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake \
  -DVITAHUD_USE_REAL_PAF=OFF
```

Real PAF mode:

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake \
  -DVITAHUD_USE_REAL_PAF=ON \
  -DVITAHUD_PAF_INCLUDE_DIR=/path/to/paf/headers \
  -DVITAHUD_PAF_LIBRARY_DIR=/path/to/paf/libs
```

Optional RCO compiler hook:

```bash
-DVITAHUD_RCO_COMPILER=/path/to/rco/compiler
```

## Current visible target

The code is aiming for:

```txt
VITAHUD PAF TEST
```

using this structure:

```cpp
PageOpen("vitahud_page_hud")
TemplateOpen(...)
FindChild(VITAHUD_TEXT_HUD_TEST)
SetString(...)
```

## Still needed

Real visible overlay still needs:

- real `paf.h`
- real PAF import libraries/stubs
- compiled `vitahud_plugin.rco`
- shell/plugin context assignment for `g_corePlugin`

## Important

This package does not include copied PSVshellPlus source files.
PSVshellPlus remains a reference for the architecture only.
