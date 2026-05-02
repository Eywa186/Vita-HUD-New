#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>

#define MODULE_START_SUCCESS 0
#define MODULE_STOP_SUCCESS 0

static int my_strlen(const char *s)
{
    int len = 0;
    if (!s) return 0;

    while (s[len] != 0) {
        len++;
    }

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

int module_start(SceSize args, void *argp)
{
    (void)args;
    (void)argp;

    log_line("VitaHUD HARD ALIVE TEST\n");
    log_line("module_start reached successfully\n");
    log_line("No thread, no PAF, no RCO, no framebuffer\n");

    return MODULE_START_SUCCESS;
}

int module_stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;

    log_line("module_stop called\n");

    return MODULE_STOP_SUCCESS;
}
