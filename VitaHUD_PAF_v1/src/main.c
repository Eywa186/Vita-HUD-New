#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/dirent.h>
#include <psp2/types.h>
#include <stdio.h>
#include <string.h>

SCE_MODULE_INFO("VitaHUDAlive", SCE_MODULE_ATTR_NONE, 1, 0);

static void write_to_path(const char *path, const char *text) {
    int fd = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777);
    if (fd >= 0) {
        sceIoWrite(fd, text, strlen(text));
        sceIoClose(fd);
    }
}

static void log_line(const char *text) {
    // Make both folders so a bad target cannot hide the test.
    sceIoMkdir("ur0:data/VitaHUD", 0777);
    sceIoMkdir("ux0:data/VitaHUD", 0777);

    write_to_path("ur0:data/VitaHUD/vitahud_alive.txt", text);
    write_to_path("ux0:data/VitaHUD/vitahud_alive_ux0_fallback.txt", text);
}

int module_start(SceSize argc, const void *args) {
    log_line("VitaHUD v6.6 HARD ALIVE LOG reached module_start.\n");
    log_line("If this file exists, taiHEN loaded ur0:tai/vitahud_paf_v5.suprx.\n");
    log_line("Next issue is visual drawing/PAF attachment, not config.\n");
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
    log_line("VitaHUD v6.6 module_stop reached.\n");
    return SCE_KERNEL_STOP_SUCCESS;
}
