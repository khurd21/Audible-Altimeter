if (NOT ${IS_PICO})

set(EXE_NAME Audible-Altimeter-Executable)

add_executable(${EXE_NAME}
    main.cpp
    )

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_enable_stdio_usb(${EXE_NAME} 1)

# pull in common dependencies
target_link_libraries(${EXE_NAME}
    Audible-Altimeter::Audible-Altimeter
    Audible-Altimeter-Embedded::Audible-Altimeter-Embedded
    )

# create map/bin/hex file etc.
pico_add_extra_outputs(${EXE_NAME})

endif()