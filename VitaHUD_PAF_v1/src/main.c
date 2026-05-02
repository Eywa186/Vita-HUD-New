#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>

static int my_strlen(const char *s)
{
    int len = 0;
    if (!s) return 0;

    while (s[len] != 0) {
        len++;
    }

    return len;
}

static void my_copy(char *dst, const char *src)
{
    int i = 0;

    if (!dst || !src) return;

    while (src[i] != 0) {
        dst[i] = src[i];
        i++;
    }

    dst[i] = 0;
}

static void my_append(char *dst, const char *src)
{
    int d = 0;
    int s = 0;

    if (!dst || !src) return;

    while (dst[d] != 0) {
        d++;
    }

    while (src[s] != 0) {
        dst[d++] = src[s++];
    }

    dst[d] = 0;
}

static void my_append_int(char *dst, int value)
{
    char temp[16];
    int i = 0;
    int j;

    if (!dst) return;

    if (value < 0) {
        my_append(dst, "-");
        value = -value;
    }

    if (value == 0) {
        my_append(dst, "0");
        return;
    }

    while (value > 0 && i < 15) {
        temp[i++] = (char)('0' + (value % 10));
        value /= 10;
    }

    for (j = i - 1; j >= 0; j--) {
        char one[2];
        one[0] = temp[j];
        one[1] = 0;
        my_append(dst, one);
    }
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

    write_text_file(
        "ur0:data/VitaHUD/vitahud_alive.txt",
        line,
        SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND
    );

    write_text_file(
        "ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt",
        line,
        SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND
    );
}

static int alive_thread(SceSize args, void *argp)
{
    int counter = 0;

    (void)args;
    (void)argp;

    log_line("InputThread: started\n");

    while (1) {
        char buffer[128];

        my_copy(buffer, "InputThread heartbeat: ");
        my_append_int(buffer, counter);
        my_append(buffer, "\n");

        log_line(buffer);

        counter++;

        sceKernelDelayThread(5 * 1000 * 1000);
    }

    return 0;
}

int module_start(SceSize args, void *argp)
{
    SceUID thid;
    char buffer[128];

    (void)args;
    (void)argp;

    log_line("VitaHUD v6.6 module_start reached\n");
    log_line("If this file exists, the SUPRX loaded successfully.\n");
    log_line("module_start: begin\n");

    thid = sceKernelCreateThread(
        "VitaHUDAliveThread",
        alive_thread,
        0x10000100,
        0x10000,
        0,
        0,
        0
    );

    my_copy(buffer, "module_start: thread id: ");
    my_append_int(buffer, thid);
    my_append(buffer, "\n");
    log_line(buffer);

    if (thid >= 0) {
        int ret = sceKernelStartThread(thid, 0, 0);

        my_copy(buffer, "module_start: start thread ret: ");
        my_append_int(buffer, ret);
        my_append(buffer, "\n");
        log_line(buffer);
    }

    log_line("module_start: success return\n");

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;

    log_line("module_stop: called\n");

    return SCE_KERNEL_STOP_SUCCESS;
}
