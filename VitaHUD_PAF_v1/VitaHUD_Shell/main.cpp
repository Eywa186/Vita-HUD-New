#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <taihen.h>

#include "global.h"
#include "hud.h"
#include "menu.h"
#include "profile.h"

SCE_MODULE_INFO("VitaHUD_PAF_v1", SCE_MODULE_ATTR_NONE, 1, 0);
SCE_MODULE_START(module_start);
SCE_MODULE_STOP(module_stop);

namespace vitahud
{
    paf::Plugin *g_corePlugin = NULL;
    Settings g_settings;

    static SceUID g_inputThread = -1;
    static bool g_running = false;

    Settings::Settings()
    {
        hudEnabled = true;
        menuOpen = false;

        hudPosition = HUD_POS_TOP_RIGHT;
        menuBg = MENU_BG_BLACK;

        showFps = true;
        showBattery = true;
        showTime = true;
        showCpu = false;
        showBus = false;
        showGpu = false;
        showAppId = false;
        showRam = false;
        showIp = false;
    }

    bool IsMenuCombo(unsigned int buttons)
    {
        return (buttons & (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_START)) ==
               (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_START);
    }

    bool IsHudToggleCombo(unsigned int buttons)
    {
        return (buttons & (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT)) ==
               (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT);
    }

    static int InputThread(SceSize args, void *argp)
    {
        (void)args;
        (void)argp;

        SceCtrlData pad;
        unsigned int lastButtons = 0;

        while (g_running) {
            sceCtrlPeekBufferPositive(0, &pad, 1);

            unsigned int buttons = pad.buttons;
            unsigned int pressed = buttons & ~lastButtons;

            if (IsMenuCombo(buttons) && !IsMenuCombo(lastButtons)) {
                g_settings.menuOpen = !g_settings.menuOpen;

                if (g_settings.menuOpen) {
                    Menu::Open();
                } else {
                    Menu::Close();
                }
            }

            if (!g_settings.menuOpen && IsHudToggleCombo(buttons) && !IsHudToggleCombo(lastButtons)) {
                g_settings.hudEnabled = !g_settings.hudEnabled;
                Hud::SetVisible(g_settings.hudEnabled);
            }

            if (g_settings.menuOpen) {
                Menu::HandleInput(buttons, pressed);
            }

            lastButtons = buttons;
            sceKernelDelayThread(16000);
        }

        return 0;
    }
}

extern "C" int module_start(SceSize args, const void *argp)
{
    (void)args;
    (void)argp;

    vitahud::LoadProfile();

    vitahud::Hud::Create();
    vitahud::Hud::SetVisible(vitahud::g_settings.hudEnabled);

    vitahud::g_running = true;
    vitahud::g_inputThread = sceKernelCreateThread(
        "VitaHUD_Input",
        vitahud::InputThread,
        0x10000100,
        0x4000,
        0,
        0,
        NULL
    );

    if (vitahud::g_inputThread >= 0) {
        sceKernelStartThread(vitahud::g_inputThread, 0, NULL);
    }

    return SCE_KERNEL_START_SUCCESS;
}

extern "C" int module_stop(SceSize args, const void *argp)
{
    (void)args;
    (void)argp;

    vitahud::g_running = false;

    if (vitahud::g_inputThread >= 0) {
        sceKernelWaitThreadEnd(vitahud::g_inputThread, NULL, NULL);
        sceKernelDeleteThread(vitahud::g_inputThread);
        vitahud::g_inputThread = -1;
    }

    vitahud::Menu::Close();
    vitahud::Hud::Destroy();

    return SCE_KERNEL_STOP_SUCCESS;
}
