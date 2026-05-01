# VitaHUD PAF v2 Visible Test

## Goal

This is the next step after the compiling v1 skeleton.

Target milestone:

```txt
VITAHUD PAF TEST
```

The project now has a cleaner PS Vita shell/PAF-style layout:

```txt
VitaHUD_PAF_v1/
  CMakeLists.txt
  README.md
  VitaHUD_Shell/
    main.cpp
    global.h
    hud.cpp
    hud.h
    menu.cpp
    menu.h
    profile.cpp
    profile.h
    paf_compat.h
    RES_RCO/
      vitahud_plugin.xml
      locale/
        vitahud_locale_en.xml
```

## Important

This is still a staged build.

The included `paf_compat.h` lets GitHub Actions compile the project in a normal VitaSDK container that does not include Sony/PAF headers.

For a real visible PAF overlay on hardware, the next step is replacing the compatibility shim with real PAF imports/headers and compiling the RCO resource.

## What v2 adds

- RCO-style resource folder
- `vitahud_plugin.xml`
- English locale XML
- cleaner HUD test text target
- `Hud::Create()` now uses a structured PageOpen/TemplateOpen style path
- `Menu::Open()` is prepared for a PAF page instead of framebuffer drawing

## Current limitations

- No RAM/IP yet
- No real PAF rendering until actual PAF/RCO wiring is available
- GitHub Actions artifact is compile proof, not final install proof

## Credits

PSVshellPlus by GrapheneCt was used as a reference for the general PS Vita shell/PAF overlay architecture and telemetry separation approach.
This VitaHUD PAF v2 package is a fresh skeleton and does not include PSVshellPlus source files.
