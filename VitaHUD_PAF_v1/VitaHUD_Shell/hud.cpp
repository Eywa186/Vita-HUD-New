#include "hud.h"
#include "profile.h"

namespace vitahud
{
    using namespace paf;
    static paf::ui::Plane *s_hudRoot = NULL;
    static paf::ui::Text *s_hudText = NULL;

    static SceUInt64 s_lastCacheTick = 0;

    static int s_cachedFps = 0;
    static int s_oldFps = -1;

    static char s_cachedLine[128] = "FPS --  BAT --%  --:--";

    static int AppendText(char *out, int pos, const char *text)
    {
        while (*text) {
            out[pos++] = *text++;
        }

        out[pos] = 0;
        return pos;
    }

    static int AppendInt(char *out, int pos, int value)
    {
        char tmp[16];
        int n = 0;
        int i;

        if (value < 0) {
            out[pos++] = '-';
            value = -value;
        }

        if (value == 0) {
            out[pos++] = '0';
            out[pos] = 0;
            return pos;
        }

        while (value > 0 && n < 15) {
            tmp[n++] = '0' + (value % 10);
            value /= 10;
        }

        for (i = n - 1; i >= 0; i--) {
            out[pos++] = tmp[i];
        }

        out[pos] = 0;
        return pos;
    }

    static void BuildTime(char *out, int *pos)
    {
        SceDateTime dt;
        sceRtcGetCurrentClockLocalTime(&dt);

        int hour = dt.hour;
        int minute = dt.minute;

        out[(*pos)++] = '0' + ((hour / 10) % 10);
        out[(*pos)++] = '0' + (hour % 10);
        out[(*pos)++] = ':';
        out[(*pos)++] = '0' + ((minute / 10) % 10);
        out[(*pos)++] = '0' + (minute % 10);
        out[*pos] = 0;
    }

    void Hud::Create()
    {
        /*
         * Phase 1 skeleton:
         * This is where the final version should open a PAF page/template and
         * locate/create text widgets.
         *
         * Intended flow:
         *
         * Plugin::PageOpenParam pgiParam;
         * pgiParam.overwrite_draw_priority = 6;
         * pageRoot = g_corePlugin->PageOpen("vitahud_page", pgiParam);
         * s_hudText = created/found Text object;
         * common::MainThreadCallList::Register(Update, NULL);
         *
         * The exact widget creation depends on your PAF/RCO setup.
         */
    }

    void Hud::Destroy()
    {
        /*
         * Unregister main-thread callback and close PAF page here.
         */
        s_hudRoot = NULL;
        s_hudText = NULL;
    }

    void Hud::SetVisible(bool visible)
    {
        if (!s_hudRoot) {
            return;
        }

        if (visible) {
            s_hudRoot->Show(common::transition::Type_Fadein1);
        } else {
            s_hudRoot->Hide(common::transition::Type_Fadeout1);
        }
    }

    void Hud::SetPosition(int position)
    {
        if (!s_hudRoot) {
            return;
        }

        paf::math::v4 pos(0.0f);

        switch (position) {
        case HUD_POS_TOP_LEFT:
            pos.set_x(10.0f);
            pos.set_y(-10.0f);
            break;

        case HUD_POS_TOP_RIGHT:
            pos.set_x(600.0f);
            pos.set_y(-10.0f);
            break;

        case HUD_POS_BOTTOM_LEFT:
            pos.set_x(10.0f);
            pos.set_y(-520.0f);
            break;

        case HUD_POS_BOTTOM_RIGHT:
        default:
            pos.set_x(600.0f);
            pos.set_y(-520.0f);
            break;
        }

        s_hudRoot->SetPos(pos);
    }

    void Hud::UpdateCachedText()
    {
        SceRtcTick tick;
        sceRtcGetCurrentTick(&tick);

        if (s_lastCacheTick != 0 && tick.tick - s_lastCacheTick < 500000) {
            return;
        }

        s_lastCacheTick = tick.tick;

        int battery = scePowerGetBatteryLifePercent();

        char line[128];
        int pos = 0;

        if (g_settings.showFps) {
            pos = AppendText(line, pos, "FPS ");
            pos = AppendInt(line, pos, s_cachedFps);
            pos = AppendText(line, pos, "  ");
        }

        if (g_settings.showBattery) {
            pos = AppendText(line, pos, "BAT ");
            pos = AppendInt(line, pos, battery);
            pos = AppendText(line, pos, "%  ");
        }

        if (g_settings.showTime) {
            BuildTime(line, &pos);
            pos = AppendText(line, pos, "  ");
        }

        if (g_settings.showCpu) {
            pos = AppendText(line, pos, "CPU ");
            pos = AppendInt(line, pos, scePowerGetArmClockFrequency());
            pos = AppendText(line, pos, "M  ");
        }

        if (g_settings.showBus) {
            pos = AppendText(line, pos, "BUS ");
            pos = AppendInt(line, pos, scePowerGetBusClockFrequency());
            pos = AppendText(line, pos, "M  ");
        }

        if (g_settings.showGpu) {
            pos = AppendText(line, pos, "GPU ");
            pos = AppendInt(line, pos, scePowerGetGpuClockFrequency());
            pos = AppendText(line, pos, "M  ");
        }

        if (g_settings.showRam) {
            pos = AppendText(line, pos, "RAM OFF  ");
        }

        if (g_settings.showIp) {
            pos = AppendText(line, pos, "IP OFF");
        }

        for (int i = 0; i < 128; i++) {
            s_cachedLine[i] = line[i];

            if (line[i] == 0) {
                break;
            }
        }
    }

    void Hud::ApplyText()
    {
        if (!s_hudText) {
            return;
        }

        /*
         * Convert cached ASCII to wide string for PAF Text.
         */
        wchar_t wbuf[128];

        int i = 0;
        while (s_cachedLine[i] && i < 127) {
            wbuf[i] = (wchar_t)s_cachedLine[i];
            i++;
        }

        wbuf[i] = 0;

        paf::wstring text = wbuf;
        s_hudText->SetString(text);
    }

    void Hud::Update(void *arg)
    {
        (void)arg;

        if (!g_settings.hudEnabled) {
            return;
        }

        /*
         * FPS source placeholder:
         * Phase 1 keeps this local. Phase 2 should use a real FPS provider.
         */
        s_cachedFps = s_oldFps >= 0 ? s_oldFps : 0;

        UpdateCachedText();
        ApplyText();
    }
}
