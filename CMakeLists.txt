# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(audible_altimeter C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
include_directories(audio_arrays)

add_executable(audible_altimeter
        main.cpp
        audio_arrays/hello_world_48k_16bit_signed_mono.h
        )

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(audible_altimeter 1)
pico_enable_stdio_usb(audible_altimeter 0)

# pull in common dependencies
target_link_libraries(audible_altimeter
    hardware_i2c
    pico_stdlib
    )

# create map/bin/hex file etc.
pico_add_extra_outputs(audible_altimeter)

# add url via pico_set_program_url