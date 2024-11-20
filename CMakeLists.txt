cmake_minimum_required(VERSION 3.28)

set(IS_PICO FALSE CACHE BOOL "Set to TRUE when building for Pico")
message(STATUS "Is Pico: ${IS_PICO}")

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
if (${IS_PICO})

    # == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
    if(WIN32)
        set(USERHOME $ENV{USERPROFILE})
    else()
        set(USERHOME $ENV{HOME})
    endif()
    set(sdkVersion 2.0.0)
    set(toolchainVersion 13_2_Rel1)
    set(picotoolVersion 2.0.0)
    set(PICO_VSCODE_CMAKE "${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake")
    if(EXISTS "${PICO_VSCODE_CMAKE}")
        include("${PICO_VSCODE_CMAKE}")
    else()
        message(WARNING "pico-vscode.cmake not found at ${PICO_VSCODE_CMAKE}. Continuing without it.")
    endif()
    # ====================================================================================

    # Pull in Raspberry Pi Pico SDK (must be before project)
    set(PICO_BOARD pico2 CACHE STRING "Tiny2350")
    include(pico_sdk_import.cmake)
    include(pico_extras_import.cmake)

endif()

project(Audible-Altimeter C CXX ASM)

if (${IS_PICO})
    pico_sdk_init()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_subdirectory(${THIRD_PARTY_DIR})
add_subdirectory(Audible-Altimeter)
add_subdirectory(Audible-Altimeter-Embedded)
add_subdirectory(Audible-Altimeter-Executable)