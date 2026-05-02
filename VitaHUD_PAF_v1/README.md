# VitaHUD PAF v6.7 Hard Alive + Framebuffer Probe

This is a diagnostic build.

It keeps the proven alive log test and adds the smallest direct framebuffer visual probe.

Expected log:

- `ur0:data/VitaHUD/vitahud_alive.txt`
- fallback: `ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt`

Expected visual result:

- A small block near the top-left of the screen during boot/shell start.

If the log appears but no box appears, the SUPRX loads but direct framebuffer drawing is not usable from this context.
