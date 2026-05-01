# Real PAF Setup

v4.1 adds:

```txt
external/paf/include/paf.h
```

This is a temporary bridge. Later, replace it with real PAF SDK headers.

Current workflow configure line:

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DVITAHUD_USE_REAL_PAF=ON \
  -DVITAHUD_PAF_INCLUDE_DIR=../external/paf/include
```

Still needed for actual overlay:
- real PAF headers
- real PAF import libs/stubs
- real RCO compile
- real g_corePlugin assignment
