# VitaHUD PAF v4.1 External PAF Header

This package adds:

```txt
external/paf/include/paf.h
```

and updates GitHub Actions to build with:

```bash
-DVITAHUD_USE_REAL_PAF=ON
-DVITAHUD_PAF_INCLUDE_DIR=../external/paf/include
```

## Important

The included `external/paf/include/paf.h` is a staged bridge header, not the final real PAF SDK.

It lets the real-mode path compile past:

```txt
fatal error: paf.h: No such file or directory
```

## Current goal

Move one step past the missing-header wall.

## Still not final

This still will not be a real visible overlay until we replace the staged bridge with real PAF headers/imports and compiled RCO wiring.

## Expected next result

GitHub Actions should now get past `paf.h`.

If it fails next, send the new error. That error will tell us the next missing real-PAF piece.
