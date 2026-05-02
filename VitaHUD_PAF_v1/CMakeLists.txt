cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    endif()
endif()

project(vitahud_paf_v5 C)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(vitahud_paf_v5
    src/main.c
)

set_target_properties(vitahud_paf_v5 PROPERTIES
    LINK_FLAGS "-nostdlib -Wl,-e,module_start"
)

target_link_libraries(vitahud_paf_v5
    SceLibKernel_stub
    SceIofilemgr_stub
    SceIofilemgrForDriver_stub
    SceDisplay_stub
    SceDisplayUser_stub
)

vita_create_self(vitahud_paf_v5.suprx vitahud_paf_v5
    UNSAFE
)
