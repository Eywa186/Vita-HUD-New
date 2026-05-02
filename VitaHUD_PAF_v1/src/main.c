#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>
#include <stdio.h>
#include <string.h>

static void write_text_file(const char *path, const char *text, int flags)
{
    SceUID fd = sceIoOpen(path, flags, 0777);

    if (fd >= 0) {
        sceIoWrite(fd, text, strlen(text));
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
    (void)args;
    (void)argp;

    log_line("InputThread: started\n");

    int counter = 0;

    while (1) {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "InputThread heartbeat: %d\n", counter++);
        log_line(buffer);

        sceKernelDelayThread(5 * 1000 * 1000);
    }

    return 0;
}

int module_start(SceSize args, void *argp)
{
    (void)args;
    (void)argp;

    log_line("VitaHUD v6.6 module_start reached\n");
    log_line("If this file exists, the SUPRX loaded successfully.\n");
    log_line("module_start: begin\n");

    SceUID thid = sceKernelCreateThread(
        "VitaHUDAliveThread",
        alive_thread,
        0x10000100,
        0x10000,
        0,
        0,
        NULL
    );

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "module_start: thread id: %d\n", thid);
    log_line(buffer);

    if (thid >= 0) {
        int ret = sceKernelStartThread(thid, 0, NULL);
        snprintf(buffer, sizeof(buffer), "module_start: start thread ret: %d\n", ret);
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
