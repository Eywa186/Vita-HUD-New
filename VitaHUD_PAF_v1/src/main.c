#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>
#include <psp2/display.h>

#define MODULE_START_SUCCESS 0
#define MODULE_STOP_SUCCESS 0

static int my_strlen(const char *s)
{
    int len = 0;
    if (!s) return 0;
    while (s[len] != 0) len++;
    return len;
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

static void log_int(const char *label, int value)
{
    char buffer[128];
    my_copy(buffer, label);
    my_append_int(buffer, value);
    my_append(buffer, "\n");
    log_line(buffer);
}

/*
   Hard framebuffer probe:
   - no PAF
   - no RCO
   - no font/text engine
   - attempts to access the current display framebuffer and paint a tiny box
   - repeats briefly so it is not instantly overwritten
*/
static void draw_probe_box(void)
{
    SceDisplayFrameBuf fb;
    int ret;
    int repeat;

    log_line("v6.7 framebuffer probe: begin\n");

    fb.size = sizeof(SceDisplayFrameBuf);
    ret = sceDisplayGetFrameBuf(&fb, SCE_DISPLAY_SETBUF_NEXTFRAME);

    log_int("sceDisplayGetFrameBuf ret: ", ret);
    log_int("fb.width: ", (int)fb.width);
    log_int("fb.height: ", (int)fb.height);
    log_int("fb.pitch: ", (int)fb.pitch);
    log_int("fb.pixelformat: ", (int)fb.pixelformat);

    if (ret < 0 || fb.base == 0) {
        log_line("v6.7 framebuffer probe: failed or null base\n");
        return;
    }

    /* Most Vita framebuffers are 32-bit pixels. This probe assumes 4 bytes per pixel. */
    for (repeat = 0; repeat < 2500; repeat++) {
        unsigned int *pixels = (unsigned int *)fb.base;
        int x;
        int y;

        /* top-left bright box, 90x42 */
        for (y = 18; y < 60; y++) {
            for (x = 18; x < 108; x++) {
                pixels[(y * fb.pitch) + x] = 0xFF00FFFF; /* ARGB-ish cyan/yellow depending format */
            }
        }

        /* smaller inner block to make it obvious */
        for (y = 28; y < 50; y++) {
            for (x = 30; x < 96; x++) {
                pixels[(y * fb.pitch) + x] = 0xFFFF0000; /* ARGB-ish red/blue depending format */
            }
        }
    }

    log_line("v6.7 framebuffer probe: draw loop finished\n");
}

int module_start(SceSize args, void *argp)
{
    (void)args;
    (void)argp;

    log_line("\nVitaHUD v6.7 HARD ALIVE + FRAMEBUFFER TEST\n");
    log_line("module_start reached successfully\n");
    log_line("No PAF, no RCO, no menu. Direct framebuffer probe only.\n");

    draw_probe_box();

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
