#pragma once

/*
 * VitaHUD PAF v1
 * Fresh shell-overlay architecture skeleton.
 *
 * This is intentionally not a framebuffer renderer.
 * The goal is to avoid fighting game/system overlays.
 */

#include <kernel.h>
#include <ctrl.h>
#include <power.h>
#include <rtc.h>

#include <paf.h>

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
