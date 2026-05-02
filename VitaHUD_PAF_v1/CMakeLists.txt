cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  endif()
endif()

project(VitaHUD_PAF_v6_6_HardAliveLog C)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaHUD Alive Log")
set(VITA_TITLEID  "VHUD00001")
set(VITA_VERSION  "01.00")

add_executable(vitahud_paf_v5
    src/main.c
)

target_link_libraries(vitahud_paf_v5
    SceLibKernel_stub
    SceIofilemgr_stub
)

vita_create_self(vitahud_paf_v5.suprx vitahud_paf_v5 UNSAFE)

add_custom_target(copy_package ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/VitaHUD_PAF_v6_6_package/ur0/tai
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/VitaHUD_PAF_v6_6_package/ur0/data/VitaHUD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/vitahud_paf_v5.suprx ${CMAKE_SOURCE_DIR}/VitaHUD_PAF_v6_6_package/ur0/tai/vitahud_paf_v5.suprx
    DEPENDS vitahud_paf_v5.suprx
)
