# VitaHUD PAF v3.1 Artifact Fix

This version fixes the GitHub Actions artifact output.

## Fixed

- Removes CMake `lib` prefix
- Forces clean output:

```txt
vitahud_paf_v3.suprx
```

- Packages a clean install-style ZIP:

```txt
VitaHUD_PAF_v3_package.zip
```

- Artifact upload now includes:
  - `vitahud_paf_v3.suprx`
  - `VitaHUD_PAF_v3_package.zip`
  - staged `RES_RCO` resources
  - package folder contents

## Package layout

```txt
VitaHUD_PAF_v3_package/
  ur0/
    tai/
      vitahud_paf_v3.suprx
    data/
      VitaHUD/
        vitahud_plugin.xml
        locale/
          vitahud_locale_en.xml
  docs/
    STATUS.txt
  INSTALL.txt
```

## Important

This still uses:

```txt
VitaHUD_Shell/paf_compat.h
```

So this is still a staged compile/package artifact, not the final visible PAF overlay.

The next phase is real PAF/RCO wiring.
