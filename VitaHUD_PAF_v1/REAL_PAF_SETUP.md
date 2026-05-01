# Real PAF Setup Checklist

Phase 4 target: show `VITAHUD PAF TEST` through the shell/PAF overlay path.

## 1. Real PAF headers

You need real headers that provide:

```cpp
#include <paf.h>
```

The normal `vitasdk/vitasdk` GitHub Actions image does not include them.

## 2. Real PAF imports/libraries

Real mode must link the same shell/PAF imports your environment expects.

CMake variables prepared:

```bash
-DVITAHUD_PAF_INCLUDE_DIR=/path/to/headers
-DVITAHUD_PAF_LIBRARY_DIR=/path/to/libs
```

## 3. RCO compilation

Current XML resource:

```txt
VitaHUD_Shell/RES_RCO/vitahud_plugin.xml
```

Target installed compiled file:

```txt
ur0:data/VitaHUD/vitahud_plugin.rco
```

CMake variable prepared:

```bash
-DVITAHUD_RCO_COMPILER=/path/to/rco/compiler
```

## 4. g_corePlugin assignment

Currently real mode has:

```cpp
paf::Plugin *g_corePlugin = NULL;
```

That must be assigned from the shell/plugin context before:

```cpp
Hud::Create();
Menu::Open();
```

## 5. First hardware test

Once real PAF is wired, first target is only:

```txt
VITAHUD PAF TEST
```

No RAM.
No IP.
No profiles.
No advanced menu.

One visible text label first.
