#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/notificationutil.h>

#define MODULE_START_SUCCESS 0
#define MODULE_STOP_SUCCESS 0
#define VITAHUD_LOG_UR0 "ur0:data/VitaHUD/vitahud_alive.txt"
#define VITAHUD_LOG_UX0 "ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt"

static int my_strlen(const char *s)
{
    int len = 0;
    if (!s) return 0;
    while (s[len] != 0) len++;
    return len;
}

static void my_copy(char *dst, const char *src)
{
    int i = 0;
    if (!dst || !src) return;
    while (src[i] != 0) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void my_append(char *dst, const char *src)
{
    int d = 0;
    int s = 0;
    if (!dst || !src) return;
    while (dst[d] != 0) d++;
    while (src[s] != 0) dst[d++] = src[s++];
    dst[d] = 0;
}

static void my_append_int(char *dst, int value)
{
    char temp[16];
    int i = 0;
    int j;
    if (!dst) return;
    if (value < 0) { my_append(dst, "-"); value = -value; }
    if (value == 0) { my_append(dst, "0"); return; }
    while (value > 0 && i < 15) { temp[i++] = (char)('0' + (value % 10)); value /= 10; }
    for (j = i - 1; j >= 0; j--) {
        char one[2]; one[0] = temp[j]; one[1] = 0; my_append(dst, one);
    }
}

static char hex_digit(int v)
{
    v &= 0xF;
    return (v < 10) ? (char)('0' + v) : (char)('A' + (v - 10));
}

static void my_append_hex_byte(char *dst, unsigned char b)
{
    char tmp[4];
    tmp[0] = hex_digit((b >> 4) & 0xF);
    tmp[1] = hex_digit(b & 0xF);
    tmp[2] = ' ';
    tmp[3] = 0;
    my_append(dst, tmp);
}

static void write_text_file(const char *path, const char *text, int flags)
{
    SceUID fd = sceIoOpen(path, flags, 0777);
    if (fd >= 0) {
        sceIoWrite(fd, text, my_strlen(text));
        sceIoClose(fd);
    }
}

static void log_line(const char *line)
{
    sceIoMkdir("ur0:data", 0777);
    sceIoMkdir("ur0:data/VitaHUD", 0777);
    sceIoMkdir("ux0:data", 0777);
    sceIoMkdir("ux0:data/VitaHUD", 0777);
    write_text_file(VITAHUD_LOG_UR0, line, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND);
    write_text_file(VITAHUD_LOG_UX0, line, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND);
}

static void log_int_line(const char *prefix, int value)
{
    char buffer[128];
    my_copy(buffer, prefix);
    my_append_int(buffer, value);
    my_append(buffer, "\n");
    log_line(buffer);
}

static void ascii_to_wchar16(SceWChar16 *out, const char *in)
{
    int i = 0;
    if (!out) return;
    while (i < 0x40F && in && in[i] != 0) {
        out[i] = (SceWChar16)((unsigned char)in[i]);
        i++;
    }
    out[i] = 0;
}

static void send_shell_notification(const char *msg)
{
    SceWChar16 text[0x410];
    int ret;

    ascii_to_wchar16(text, msg);

    ret = sceNotificationUtilBgAppInitialize();
    log_int_line("sceNotificationUtilBgAppInitialize ret: ", ret);

    ret = sceNotificationUtilSendNotification(text);
    log_int_line("sceNotificationUtilSendNotification ret: ", ret);
}

static void probe_file(const char *path)
{
    char buffer[256];
    unsigned char bytes[16];
    int read_ret = -9999;
    SceUID fd;
    int i;

    my_copy(buffer, "probe open: ");
    my_append(buffer, path);
    my_append(buffer, "\n");
    log_line(buffer);

    fd = sceIoOpen(path, SCE_O_RDONLY, 0);
    log_int_line("  fd: ", fd);

    if (fd >= 0) {
        read_ret = sceIoRead(fd, bytes, 16);
        log_int_line("  read first 16 ret: ", read_ret);

        my_copy(buffer, "  first bytes: ");
        for (i = 0; i < read_ret && i < 16; i++) {
            my_append_hex_byte(buffer, bytes[i]);
        }
        my_append(buffer, "\n");
        log_line(buffer);

        sceIoClose(fd);
    }
}

static int shell_visual_thread(SceSize args, void *argp)
{
    (void)args;
    (void)argp;

    log_line("v7.0 shell visual thread started\n");

    sceKernelDelayThread(5 * 1000 * 1000);
    log_line("v7.0 delayed phase: shell should be ready now\n");

    send_shell_notification("VitaHUD v7.0 loaded - shell visual test");

    probe_file("ur0:data/VitaHUD/sample_plugin.rco");
    probe_file("ur0:data/VitaHUD/sample_plugin.xml");
    probe_file("ur0:data/VitaHUD/vitahud_plugin.xml");

    log_line("v7.0 note: notification path tested. True PAF widget attach requires restoring external/vdsuite headers/libs.\n");
    log_line("v7.0 shell visual thread finished\n");

    return 0;
}

int module_start(SceSize args, void *argp)
{
    SceUID thid;

    (void)args;
    (void)argp;

    log_line("\nVitaHUD v7.0 SHELL VISUAL + PAF RESOURCE LOGGER\n");
    log_line("module_start reached successfully\n");
    log_line("No framebuffer. Testing shell notification visual path + RCO resource readability.\n");

    thid = sceKernelCreateThread(
        "VitaHUDShellVisual",
        shell_visual_thread,
        0x10000100,
        0x10000,
        0,
        0,
        0
    );

    log_int_line("sceKernelCreateThread ret: ", thid);

    if (thid >= 0) {
        int ret = sceKernelStartThread(thid, 0, 0);
        log_int_line("sceKernelStartThread ret: ", ret);
    }

    log_line("module_start success return\n");
    return MODULE_START_SUCCESS;
}

int module_stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    log_line("module_stop called\n");
    return MODULE_STOP_SUCCESS;
}
