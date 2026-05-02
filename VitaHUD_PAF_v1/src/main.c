#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>
#include <psp2/display.h>
#include <psp2/kernel/threadmgr.h>

#ifndef SCE_DISPLAY_SETBUF_NEXTFRAME
#define SCE_DISPLAY_SETBUF_NEXTFRAME 0
#endif

#define MODULE_START_SUCCESS 0
#define MODULE_STOP_SUCCESS 0

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

    write_text_file("ur0:data/VitaHUD/vitahud_alive.txt", line, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND);
    write_text_file("ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt", line, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND);
}

static void log_int_line(const char *prefix, int value)
{
    char buffer[128];
    my_copy(buffer, prefix);
    my_append_int(buffer, value);
    my_append(buffer, "\n");
    log_line(buffer);
}

static void draw_box(unsigned int *base, int pitch, int x, int y, int w, int h, unsigned int color)
{
    int yy;
    int xx;

    for (yy = 0; yy < h; yy++) {
        unsigned int *row = base + ((y + yy) * pitch) + x;
        for (xx = 0; xx < w; xx++) {
            row[xx] = color;
        }
    }
}

static int live_screen_thread(SceSize args, void *argp)
{
    int tick = 0;
    int good_frames = 0;

    (void)args;
    (void)argp;

    log_line("v6.9 live screen thread started\n");

    while (tick < 600) {
        SceDisplayFrameBuf fb;
        int ret;

        fb.size = sizeof(SceDisplayFrameBuf);
        fb.base = 0;
        fb.pitch = 0;
        fb.width = 0;
        fb.height = 0;
        fb.pixelformat = 0;

        ret = sceDisplayGetFrameBuf(&fb, SCE_DISPLAY_SETBUF_NEXTFRAME);

        if ((tick % 30) == 0) {
            log_int_line("sceDisplayGetFrameBuf ret: ", ret);
            log_int_line("fb.width: ", (int)fb.width);
            log_int_line("fb.height: ", (int)fb.height);
            log_int_line("fb.pitch: ", (int)fb.pitch);
            log_int_line("fb.pixelformat: ", (int)fb.pixelformat);
        }

        if (ret >= 0 && fb.base != 0 && fb.pitch > 0) {
            unsigned int color;
            int x = 20;
            int y = 20;
            int w = 96;
            int h = 48;

            good_frames++;

            if ((tick & 1) == 0) {
                color = 0xFFFF00FF; /* bright magenta */
            } else {
                color = 0xFF00FFFF; /* cyan/yellow depending format, still obvious */
            }

            draw_box((unsigned int *)fb.base, (int)fb.pitch, x, y, w, h, color);
            draw_box((unsigned int *)fb.base, (int)fb.pitch, x + 8, y + 8, w - 16, h - 16, 0xFF000000);
        }

        tick++;
        sceKernelDelayThread(100 * 1000); /* 100ms */
    }

    log_int_line("v6.9 live screen thread finished. good_frames: ", good_frames);
    return 0;
}

int module_start(SceSize args, void *argp)
{
    SceUID thid;

    (void)args;
    (void)argp;

    log_line("\nVitaHUD v6.9 LIVE SCREEN TEST\n");
    log_line("module_start reached successfully\n");
    log_line("Starting live framebuffer thread. Look top-left for flashing box.\n");

    thid = sceKernelCreateThread(
        "VitaHUDLiveScreen",
        live_screen_thread,
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
