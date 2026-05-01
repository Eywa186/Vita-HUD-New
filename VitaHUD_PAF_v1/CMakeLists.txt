cmake_minimum_required(VERSION 3.10)

project(VitaHUD_PAF_v4 C CXX)

set(CMAKE_CXX_STANDARD 11)

option(VITAHUD_USE_REAL_PAF "Build using real PAF headers/imports instead of paf_compat.h" OFF)

set(VITAHUD_PAF_INCLUDE_DIR "" CACHE PATH "Path to real PAF headers")
set(VITAHUD_PAF_LIBRARY_DIR "" CACHE PATH "Path to real PAF libraries/imports")
set(VITAHUD_RCO_COMPILER "" CACHE FILEPATH "Optional RCO compiler/tool path")

add_library(VitaHUD_PAF_v4 MODULE
    VitaHUD_Shell/main.cpp
    VitaHUD_Shell/hud.cpp
    VitaHUD_Shell/menu.cpp
    VitaHUD_Shell/profile.cpp
)

target_include_directories(VitaHUD_PAF_v4 PRIVATE
    VitaHUD_Shell
)

if(VITAHUD_USE_REAL_PAF)
    target_compile_definitions(VitaHUD_PAF_v4 PRIVATE VITAHUD_USE_REAL_PAF=1)

    if(VITAHUD_PAF_INCLUDE_DIR)
        target_include_directories(VitaHUD_PAF_v4 PRIVATE ${VITAHUD_PAF_INCLUDE_DIR})
    endif()

    if(VITAHUD_PAF_LIBRARY_DIR)
        target_link_directories(VitaHUD_PAF_v4 PRIVATE ${VITAHUD_PAF_LIBRARY_DIR})
    endif()

    message(STATUS "VitaHUD real PAF mode enabled")
else()
    message(STATUS "VitaHUD compatibility PAF mode enabled")
endif()

target_link_libraries(VitaHUD_PAF_v4
    SceLibKernel_stub
    SceKernelThreadMgr_stub
    SceCtrl_stub
    ScePower_stub
    SceRtc_stub
    SceIofilemgr_stub
)

set_target_properties(VitaHUD_PAF_v4 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "vitahud_paf_v4"
    SUFFIX ".suprx"
)

# Optional future RCO compilation hook.
# Current repo stages XML resources only.
if(VITAHUD_RCO_COMPILER)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vitahud_plugin.rco
        COMMAND ${VITAHUD_RCO_COMPILER}
                ${CMAKE_CURRENT_SOURCE_DIR}/VitaHUD_Shell/RES_RCO/vitahud_plugin.xml
                ${CMAKE_CURRENT_BINARY_DIR}/vitahud_plugin.rco
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/VitaHUD_Shell/RES_RCO/vitahud_plugin.xml
        COMMENT "Compiling VitaHUD RCO"
    )

    add_custom_target(vitahud_rco ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/vitahud_plugin.rco
    )
endif()
