VitaHUD PAF v6.6 HARD ALIVE LOG

This build intentionally does NOT draw anything.
It only proves whether taiHEN is loading the plugin.

Install:
1. Copy vitahud_paf_v5.suprx to:
   ur0:tai/vitahud_paf_v5.suprx

2. In ur0:tai/config.txt, under *main, place this as the FIRST plugin line:
   ur0:tai/vitahud_paf_v5.suprx

3. Full reboot.

4. Check for:
   ur0:data/VitaHUD/vitahud_alive.txt

Fallback check:
   ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt

Meaning:
- If either file exists: plugin loaded. Config/path are good. Visual path is the issue.
- If neither file exists: taiHEN did not load the plugin. Check config section, filename, reboot, and plugin compatibility.
