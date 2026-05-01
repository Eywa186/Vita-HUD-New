cmake_minimum_required(VERSION 3.10)

project(VitaHUD_PAF_v1 C CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(VitaHUD_PAF_v1 MODULE
    VitaHUD_Shell/main.cpp
    VitaHUD_Shell/hud.cpp
    VitaHUD_Shell/menu.cpp
    VitaHUD_Shell/profile.cpp
)

target_include_directories(VitaHUD_PAF_v1 PRIVATE
    VitaHUD_Shell
)

# This is a skeleton. PAF/plugin linking varies by SDK/toolchain setup.
# You will likely need to adjust PAF/Core shell imports to match your environment.
target_link_libraries(VitaHUD_PAF_v1
    SceLibKernel_stub
    SceKernelThreadMgr_stub
    SceCtrl_stub
    ScePower_stub
    SceRtc_stub
    SceIofilemgr_stub
)

set_target_properties(VitaHUD_PAF_v1 PROPERTIES
    OUTPUT_NAME "vitahud_paf_v1"
    SUFFIX ".suprx"
)
