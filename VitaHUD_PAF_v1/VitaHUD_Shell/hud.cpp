#include "hud.h"
#include "profile.h"

namespace vitahud
{
    using namespace paf;

    static paf::ui::Plane *s_hudPage = NULL;
    static paf::ui::Plane *s_hudRoot = NULL;
    static paf::ui::Text *s_hudText = NULL;

    static SceUInt64 s_lastCacheTick = 0;
    static char s_cachedLine[128] = "VITAHUD PAF TEST";

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
        if (!g_corePlugin) {
            return;
        }

        Plugin::PageOpenParam pageParam;
        Plugin::TemplateOpenParam templateParam;

        /*
         * Real PAF target:
         * This is the important structure we wanted:
         * - open a PAF page
         * - set draw priority
         * - open a HUD template
         * - find the HUD text object
         */
        pageParam.overwrite_draw_priority = 6;
        s_hudPage = g_corePlugin->PageOpen("vitahud_page_hud", pageParam);

        if (!s_hudPage) {
            return;
        }

        g_corePlugin->TemplateOpen(s_hudPage, VITAHUD_TEMPLATE_HUD_TEST, templateParam);

        s_hudRoot = s_hudPage->GetChild(s_hudPage->GetChildrenNum() - 1);
        s_hudText = (paf::ui::Text *)s_hudRoot->FindChild(VITAHUD_TEXT_HUD_TEST);

        SetPosition(g_settings.hudPosition);
        SetVisible(g_settings.hudEnabled);
        Update(NULL);
    }

    void Hud::Destroy()
    {
        if (g_corePlugin && s_hudPage) {
            Plugin::PageCloseParam closeParam;
            closeParam.fade = true;
            g_corePlugin->PageClose("vitahud_page_hud", closeParam);
        }

        s_hudPage = NULL;
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

        pos = AppendText(line, pos, "VITAHUD PAF TEST  ");

        if (g_settings.showBattery) {
            pos = AppendText(line, pos, "BAT ");
            pos = AppendInt(line, pos, battery);
            pos = AppendText(line, pos, "%  ");
        }

        if (g_settings.showTime) {
            BuildTime(line, &pos);
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

        UpdateCachedText();
        ApplyText();
    }
}
