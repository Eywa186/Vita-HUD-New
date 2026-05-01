cmake_minimum_required(VERSION 3.10)

project(VitaHUD_PAF_v2 C CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(VitaHUD_PAF_v2 MODULE
    VitaHUD_Shell/main.cpp
    VitaHUD_Shell/hud.cpp
    VitaHUD_Shell/menu.cpp
    VitaHUD_Shell/profile.cpp
)

target_include_directories(VitaHUD_PAF_v2 PRIVATE
    VitaHUD_Shell
)

target_link_libraries(VitaHUD_PAF_v2
    SceLibKernel_stub
    SceKernelThreadMgr_stub
    SceCtrl_stub
    ScePower_stub
    SceRtc_stub
    SceIofilemgr_stub
)

set_target_properties(VitaHUD_PAF_v2 PROPERTIES
    OUTPUT_NAME "vitahud_paf_v2"
    SUFFIX ".suprx"
)

# RCO resource files are staged here for the real PAF wiring phase.
# They are not compiled by this CMake file yet because the normal
# GitHub VitaSDK container does not include the full PAF/RCO pipeline.
