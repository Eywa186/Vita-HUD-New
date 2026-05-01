# VitaHUD PAF v3 Packaged Artifact

This version keeps the successful v2 compile base and adds a clean packaged artifact output.

## Current purpose

This is still a staged build. It proves the project can:

- compile in GitHub Actions
- build the C++ VitaHUD PAF structure
- stage RES_RCO files
- produce a clean package zip

## Important

This still uses:

```txt
VitaHUD_Shell/paf_compat.h
```

That means this is **not the final visible PAF overlay yet**.

The next phase is real PAF/RCO wiring.

## GitHub Actions

Use the included workflow:

```txt
.github/workflows/build.yml
```

It builds from:

```txt
VitaHUD_PAF_v1/
```

and creates:

```txt
VitaHUD_PAF_v1/build/VitaHUD_PAF_v3_package.zip
```

## Package layout

The package zip contains:

```txt
VitaHUD_PAF_v3_package/
  ur0/
    tai/
      vitahud_paf_v2.suprx
    data/
      VitaHUD/
        vitahud_plugin.xml
        locale/
          vitahud_locale_en.xml
  docs/
    STATUS.txt
  INSTALL.txt
```

## Status

Working:

- VitaSDK build
- GitHub Actions artifact
- package output
- staged RCO/XML resources
- HUD/Menu source structure
- profile source structure

Not final yet:

- real PAF headers/imports
- compiled `.rco`
- real visible overlay
- RAM/IP telemetry
