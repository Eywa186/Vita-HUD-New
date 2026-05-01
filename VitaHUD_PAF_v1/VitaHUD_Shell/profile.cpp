#include <io/fcntl.h>
#include <io/stat.h>

#include "profile.h"
#include "global.h"

namespace vitahud
{
    static const char *PROFILE_DIR = "ux0:data/VitaHUD";
    static const char *PROFILE_PATH = "ux0:data/VitaHUD/paf_profile1.txt";

    static void WriteLine(SceUID fd, const char *key, int value)
    {
        char line[64];
        int pos = 0;

        while (*key) {
            line[pos++] = *key++;
        }

        line[pos++] = '=';

        if (value == 0) {
            line[pos++] = '0';
        } else {
            char tmp[16];
            int n = 0;
            int v = value;

            if (v < 0) {
                line[pos++] = '-';
                v = -v;
            }

            while (v > 0 && n < 15) {
                tmp[n++] = '0' + (v % 10);
                v /= 10;
            }

            for (int i = n - 1; i >= 0; i--) {
                line[pos++] = tmp[i];
            }
        }

        line[pos++] = '\n';
        sceIoWrite(fd, line, pos);
    }

    static int ReadInt(const char *buf, const char *key, int fallback)
    {
        const char *p = buf;

        while (*p) {
            const char *a = p;
            const char *b = key;

            while (*a && *b && *a == *b) {
                a++;
                b++;
            }

            if (*b == 0 && *a == '=') {
                a++;

                int sign = 1;
                int value = 0;

                if (*a == '-') {
                    sign = -1;
                    a++;
                }

                while (*a >= '0' && *a <= '9') {
                    value = value * 10 + (*a - '0');
                    a++;
                }

                return value * sign;
            }

            while (*p && *p != '\n') {
                p++;
            }

            if (*p == '\n') {
                p++;
            }
        }

        return fallback;
    }

    void SaveProfile()
    {
        sceIoMkdir(PROFILE_DIR, 0777);

        SceUID fd = sceIoOpen(PROFILE_PATH, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);

        if (fd < 0) {
            return;
        }

        WriteLine(fd, "hudEnabled", g_settings.hudEnabled ? 1 : 0);
        WriteLine(fd, "hudPosition", g_settings.hudPosition);
        WriteLine(fd, "menuBg", g_settings.menuBg);

        WriteLine(fd, "showFps", g_settings.showFps ? 1 : 0);
        WriteLine(fd, "showBattery", g_settings.showBattery ? 1 : 0);
        WriteLine(fd, "showTime", g_settings.showTime ? 1 : 0);
        WriteLine(fd, "showCpu", g_settings.showCpu ? 1 : 0);
        WriteLine(fd, "showBus", g_settings.showBus ? 1 : 0);
        WriteLine(fd, "showGpu", g_settings.showGpu ? 1 : 0);
        WriteLine(fd, "showAppId", g_settings.showAppId ? 1 : 0);
        WriteLine(fd, "showRam", g_settings.showRam ? 1 : 0);
        WriteLine(fd, "showIp", g_settings.showIp ? 1 : 0);

        sceIoClose(fd);
    }

    void LoadProfile()
    {
        char buf[1024];

        SceUID fd = sceIoOpen(PROFILE_PATH, SCE_O_RDONLY, 0);

        if (fd < 0) {
            return;
        }

        int read = sceIoRead(fd, buf, sizeof(buf) - 1);
        sceIoClose(fd);

        if (read <= 0) {
            return;
        }

        buf[read] = 0;

        g_settings.hudEnabled = ReadInt(buf, "hudEnabled", g_settings.hudEnabled ? 1 : 0) != 0;
        g_settings.hudPosition = ReadInt(buf, "hudPosition", g_settings.hudPosition);
        g_settings.menuBg = ReadInt(buf, "menuBg", g_settings.menuBg);

        g_settings.showFps = ReadInt(buf, "showFps", g_settings.showFps ? 1 : 0) != 0;
        g_settings.showBattery = ReadInt(buf, "showBattery", g_settings.showBattery ? 1 : 0) != 0;
        g_settings.showTime = ReadInt(buf, "showTime", g_settings.showTime ? 1 : 0) != 0;
        g_settings.showCpu = ReadInt(buf, "showCpu", g_settings.showCpu ? 1 : 0) != 0;
        g_settings.showBus = ReadInt(buf, "showBus", g_settings.showBus ? 1 : 0) != 0;
        g_settings.showGpu = ReadInt(buf, "showGpu", g_settings.showGpu ? 1 : 0) != 0;
        g_settings.showAppId = ReadInt(buf, "showAppId", g_settings.showAppId ? 1 : 0) != 0;
        g_settings.showRam = ReadInt(buf, "showRam", g_settings.showRam ? 1 : 0) != 0;
        g_settings.showIp = ReadInt(buf, "showIp", g_settings.showIp ? 1 : 0) != 0;
    }
}
