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

# build parameters:
set(PICO_BOARD pico2 CACHE STRING "Board type")
set(PROJECT_NAME audible_altimeter)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# this can't be set with ${PROJECT_NAME} bc rpi foundation parses it and uses it to find the
# executable name
project(audible_altimeter C CXX ASM)

# this allows c-asserts to track illegal parameters
add_compile_definitions(PARAM_ASSERTIONS_ENABLE_ALL=1)
# add_compile_definitions(PARAM_ASSERTIONS_DISABLE_ALL=1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${PROJECT_NAME}
        main.cpp
        audio_arrays/audio_samples.h
        audio_arrays/two.cpp
        audio_arrays/three.cpp
        audio_arrays/audio_test_48k_16bit_mono.cpp
        audio_arrays/one.cpp
    )

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/pio_serializer.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    # audio_samples
    hardware_gpio
    hardware_pio
    hardware_clocks
    # hardware_i2c
    hardware_dma
    pico_stdlib
    hardware_irq
    )

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PICO_AUDIO_I2S_DATA_PIN=4
    PICO_AUDIO_I2S_CLOCK_PIN_BASE=2 # bit clock and then lrc
    )

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

set(CMAKE_SIZE arm-none-eabi-size)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf
    COMMENT "Binary size information"
)