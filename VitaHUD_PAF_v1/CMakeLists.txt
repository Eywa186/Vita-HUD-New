cmake_minimum_required(VERSION 3.10)

project(VitaHUD_PAF_v3 C CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(VitaHUD_PAF_v3 MODULE
    VitaHUD_Shell/main.cpp
    VitaHUD_Shell/hud.cpp
    VitaHUD_Shell/menu.cpp
    VitaHUD_Shell/profile.cpp
)

target_include_directories(VitaHUD_PAF_v3 PRIVATE
    VitaHUD_Shell
)

target_link_libraries(VitaHUD_PAF_v3
    SceLibKernel_stub
    SceKernelThreadMgr_stub
    SceCtrl_stub
    ScePower_stub
    SceRtc_stub
    SceIofilemgr_stub
)

set_target_properties(VitaHUD_PAF_v3 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "vitahud_paf_v3"
    SUFFIX ".suprx"
)

# RES_RCO resources are staged for packaging.
# Real RCO compilation will be added when real PAF tooling is wired.
