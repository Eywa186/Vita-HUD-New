#pragma once

/*
 * VitaHUD real PAF wiring notes
 *
 * This file intentionally contains no copied PSVshellPlus implementation.
 *
 * Required real-mode pieces:
 * - real paf.h headers available to compiler
 * - real PAF import libraries/stubs available to linker
 * - compiled vitahud_plugin.rco installed at ur0:data/VitaHUD/
 * - g_corePlugin assigned from shell/plugin context, not fallback shim
 *
 * Current visible target:
 * - Page:     vitahud_page_hud
 * - Template: VITAHUD_TEMPLATE_HUD_TEST
 * - Text:     VITAHUD_TEXT_HUD_TEST
 */
