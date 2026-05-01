#pragma once

/*
 * VitaHUD PAF v2
 * Header-path fixed for normal VitaSDK builds.
 * Real PAF wiring will replace paf_compat.h later.
 */

#include <psp2/types.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>

#include "paf_compat.h"

namespace vitahud
{
    extern paf::Plugin *g_corePlugin;

    enum HudPosition {
        HUD_POS_TOP_LEFT = 0,
        HUD_POS_TOP_RIGHT,
        HUD_POS_BOTTOM_LEFT,
        HUD_POS_BOTTOM_RIGHT
    };

    enum MenuBg {
        MENU_BG_BLACK = 0,
        MENU_BG_GRAY,
        MENU_BG_NAVY,
        MENU_BG_GREEN,
        MENU_BG_PURPLE,
        MENU_BG_TRANSPARENT,
        MENU_BG_COUNT
    };

    enum ResourceIds {
        VITAHUD_TEMPLATE_HUD_TEST = 1000,
        VITAHUD_TEMPLATE_MENU_TEST = 1001,
        VITAHUD_TEXT_HUD_TEST = 2000,
        VITAHUD_TEXT_MENU_TEST = 2001
    };

    struct Settings {
        bool hudEnabled;
        bool menuOpen;

        int hudPosition;
        int menuBg;

        bool showFps;
        bool showBattery;
        bool showTime;
        bool showCpu;
        bool showBus;
        bool showGpu;
        bool showAppId;
        bool showRam;
        bool showIp;

        Settings();
    };

    extern Settings g_settings;

    bool IsMenuCombo(unsigned int buttons);
    bool IsHudToggleCombo(unsigned int buttons);
}
