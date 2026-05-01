#include "menu.h"
#include "hud.h"
#include "profile.h"

namespace vitahud
{
    using namespace paf;

    static paf::ui::Plane *s_menuPage = NULL;
    static paf::ui::Plane *s_menuRoot = NULL;
    static paf::ui::Text *s_menuText = NULL;

    static int s_index = 0;
    static int s_scroll = 0;

    enum MenuItem {
        MENU_HUD = 0,
        MENU_POSITION,
        MENU_FPS,
        MENU_BATTERY,
        MENU_TIME,
        MENU_CPU,
        MENU_BUS,
        MENU_GPU,
        MENU_APP_ID,
        MENU_RAM,
        MENU_IP,
        MENU_BG,
        MENU_SAVE_PROFILE,
        MENU_LOAD_PROFILE,
        MENU_RESET,
        MENU_COUNT
    };

    static const char *OnOff(bool value)
    {
        return value ? "ON" : "OFF";
    }

    static const char *MenuBgName()
    {
        switch (g_settings.menuBg) {
        case MENU_BG_GRAY: return "GRAY";
        case MENU_BG_NAVY: return "NAVY";
        case MENU_BG_GREEN: return "GREEN";
        case MENU_BG_PURPLE: return "PURPLE";
        case MENU_BG_TRANSPARENT: return "TRANSPARENT";
        case MENU_BG_BLACK:
        default: return "BLACK";
        }
    }

    static const char *PositionName()
    {
        switch (g_settings.hudPosition) {
        case HUD_POS_TOP_LEFT: return "TOP LEFT";
        case HUD_POS_BOTTOM_LEFT: return "BOTTOM LEFT";
        case HUD_POS_BOTTOM_RIGHT: return "BOTTOM RIGHT";
        case HUD_POS_TOP_RIGHT:
        default: return "TOP RIGHT";
        }
    }

    static const char *Label(int item)
    {
        switch (item) {
        case MENU_HUD: return "HUD";
        case MENU_POSITION: return "POSITION";
        case MENU_FPS: return "FPS";
        case MENU_BATTERY: return "BATTERY";
        case MENU_TIME: return "CLOCK";
        case MENU_CPU: return "CPU HUD";
        case MENU_BUS: return "BUS HUD";
        case MENU_GPU: return "GPU HUD";
        case MENU_APP_ID: return "APP ID HUD";
        case MENU_RAM: return "RAM HUD";
        case MENU_IP: return "IP HUD";
        case MENU_BG: return "MENU BG";
        case MENU_SAVE_PROFILE: return "SAVE PROFILE";
        case MENU_LOAD_PROFILE: return "LOAD PROFILE";
        case MENU_RESET: return "RESET DEFAULTS";
        default: return "";
        }
    }

    static const char *Value(int item)
    {
        switch (item) {
        case MENU_HUD: return OnOff(g_settings.hudEnabled);
        case MENU_POSITION: return PositionName();
        case MENU_FPS: return OnOff(g_settings.showFps);
        case MENU_BATTERY: return OnOff(g_settings.showBattery);
        case MENU_TIME: return OnOff(g_settings.showTime);
        case MENU_CPU: return OnOff(g_settings.showCpu);
        case MENU_BUS: return OnOff(g_settings.showBus);
        case MENU_GPU: return OnOff(g_settings.showGpu);
        case MENU_APP_ID: return OnOff(g_settings.showAppId);
        case MENU_RAM: return OnOff(g_settings.showRam);
        case MENU_IP: return OnOff(g_settings.showIp);
        case MENU_BG: return MenuBgName();
        case MENU_SAVE_PROFILE: return "PRESS X";
        case MENU_LOAD_PROFILE: return "PRESS X";
        case MENU_RESET: return "PRESS X";
        default: return "";
        }
    }

    static void AppendAscii(wchar_t *out, int *pos, const char *text)
    {
        while (*text && *pos < 1023) {
            out[(*pos)++] = (wchar_t)*text++;
        }

        out[*pos] = 0;
    }

    void Menu::Open()
    {
        if (!g_corePlugin) {
            return;
        }

        if (!s_menuPage) {
            Plugin::PageOpenParam pageParam;
            Plugin::TemplateOpenParam templateParam;

            pageParam.overwrite_draw_priority = 7;
            s_menuPage = g_corePlugin->PageOpen("vitahud_page_menu", pageParam);

            if (s_menuPage) {
                g_corePlugin->TemplateOpen(s_menuPage, VITAHUD_TEMPLATE_MENU_TEST, templateParam);
                s_menuRoot = s_menuPage->GetChild(s_menuPage->GetChildrenNum() - 1);
                s_menuText = (paf::ui::Text *)s_menuRoot->FindChild(VITAHUD_TEXT_MENU_TEST);
            }
        }

        if (s_menuRoot) {
            s_menuRoot->Show(common::transition::Type_Fadein1);
        }

        RebuildText();
    }

    void Menu::Close()
    {
        if (s_menuRoot) {
            s_menuRoot->Hide(common::transition::Type_Fadeout1);
        }
    }

    void Menu::HandleInput(unsigned int buttons, unsigned int pressed)
    {
        (void)buttons;

        if (pressed & SCE_CTRL_UP) {
            s_index--;

            if (s_index < 0) {
                s_index = MENU_COUNT - 1;
            }

            RebuildText();
        }

        if (pressed & SCE_CTRL_DOWN) {
            s_index++;

            if (s_index >= MENU_COUNT) {
                s_index = 0;
            }

            RebuildText();
        }

        if (pressed & SCE_CTRL_LEFT) {
            ChangeCurrent(-1);
            RebuildText();
        }

        if (pressed & SCE_CTRL_RIGHT) {
            ChangeCurrent(1);
            RebuildText();
        }

        if (pressed & SCE_CTRL_CROSS) {
            ChangeCurrent(1);
            RebuildText();
        }

        if (pressed & SCE_CTRL_CIRCLE) {
            g_settings.menuOpen = false;
            Close();
        }
    }

    void Menu::ChangeCurrent(int direction)
    {
        switch (s_index) {
        case MENU_HUD:
            g_settings.hudEnabled = !g_settings.hudEnabled;
            Hud::SetVisible(g_settings.hudEnabled);
            break;

        case MENU_POSITION:
            g_settings.hudPosition += direction;
            if (g_settings.hudPosition < 0) {
                g_settings.hudPosition = HUD_POS_BOTTOM_RIGHT;
            }
            if (g_settings.hudPosition > HUD_POS_BOTTOM_RIGHT) {
                g_settings.hudPosition = HUD_POS_TOP_LEFT;
            }
            Hud::SetPosition(g_settings.hudPosition);
            break;

        case MENU_FPS:
            g_settings.showFps = !g_settings.showFps;
            break;

        case MENU_BATTERY:
            g_settings.showBattery = !g_settings.showBattery;
            break;

        case MENU_TIME:
            g_settings.showTime = !g_settings.showTime;
            break;

        case MENU_CPU:
            g_settings.showCpu = !g_settings.showCpu;
            break;

        case MENU_BUS:
            g_settings.showBus = !g_settings.showBus;
            break;

        case MENU_GPU:
            g_settings.showGpu = !g_settings.showGpu;
            break;

        case MENU_APP_ID:
            g_settings.showAppId = !g_settings.showAppId;
            break;

        case MENU_RAM:
            g_settings.showRam = !g_settings.showRam;
            break;

        case MENU_IP:
            g_settings.showIp = !g_settings.showIp;
            break;

        case MENU_BG:
            g_settings.menuBg += direction;
            if (g_settings.menuBg < 0) {
                g_settings.menuBg = MENU_BG_COUNT - 1;
            }
            if (g_settings.menuBg >= MENU_BG_COUNT) {
                g_settings.menuBg = 0;
            }
            break;

        case MENU_SAVE_PROFILE:
            SaveProfile();
            break;

        case MENU_LOAD_PROFILE:
            LoadProfile();
            Hud::SetPosition(g_settings.hudPosition);
            Hud::SetVisible(g_settings.hudEnabled);
            break;

        case MENU_RESET:
            g_settings = Settings();
            Hud::SetPosition(g_settings.hudPosition);
            Hud::SetVisible(g_settings.hudEnabled);
            break;
        }
    }

    void Menu::RebuildText()
    {
        if (!s_menuText) {
            return;
        }

        wchar_t wbuf[1024];
        int pos = 0;

        AppendAscii(wbuf, &pos, "VITAHUD PAF TEST MENU\\n");
        AppendAscii(wbuf, &pos, "---------------------\\n");

        int visible = 11;

        if (s_index < s_scroll) {
            s_scroll = s_index;
        }

        if (s_index >= s_scroll + visible) {
            s_scroll = s_index - visible + 1;
        }

        for (int i = s_scroll; i < MENU_COUNT && i < s_scroll + visible; i++) {
            AppendAscii(wbuf, &pos, i == s_index ? "> " : "  ");
            AppendAscii(wbuf, &pos, Label(i));
            AppendAscii(wbuf, &pos, " : ");
            AppendAscii(wbuf, &pos, Value(i));
            AppendAscii(wbuf, &pos, "\\n");
        }

        AppendAscii(wbuf, &pos, "---------------------\\n");
        AppendAscii(wbuf, &pos, "UP/DOWN MOVE  LEFT/RIGHT CHANGE  O CLOSE");

        paf::wstring text = wbuf;
        s_menuText->SetString(text);
    }

    void Menu::Update(void *arg)
    {
        (void)arg;

        if (g_settings.menuOpen) {
            RebuildText();
        }
    }
}
