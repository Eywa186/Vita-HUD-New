# VitaHUD PAF v6.6 Hard Alive Log

This is not a visual HUD build.

It is a hard loading diagnostic build. It proves whether taiHEN actually loads `ur0:tai/vitahud_paf_v5.suprx`.

When loaded, it immediately writes:

```txt
ur0:data/VitaHUD/vitahud_alive.txt
```

Fallback:

```txt
ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt
```

If either file exists after reboot, the plugin loaded and the config/path are good.
If neither file exists, taiHEN did not load the plugin.
