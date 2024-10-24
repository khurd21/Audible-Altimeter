set(LIB_NAME Audible-Altimeter-Embedded)


set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(sources
    rp2350_i2s_driver.cpp
    board_descriptions.hpp
    bmp_390.cpp
    bmp_390_communications.hpp
    bmp_390_communications.cpp
    tiny_2350.cpp
    timer.cpp
    )
set(includes
    ${include_dir}/Audible-Altimeter-Embedded/tiny_2350.hpp
    ${include_dir}/Audible-Altimeter-Embedded/bmp_390.hpp
    ${include_dir}/Audible-Altimeter-Embedded/timer.hpp
    ${include_dir}/Audible-Altimeter-Embedded/rp2350_i2s_driver.hpp
    )

if (${IS_PICO})
    add_library(${LIB_NAME} STATIC ${sources} ${includes})
    add_library(${LIB_NAME}::Audible-Altimeter-Embedded ALIAS ${LIB_NAME})
    target_include_directories(${LIB_NAME} PUBLIC ${include_dir})
    target_link_libraries(${LIB_NAME} PUBLIC
        Audible-Altimeter::Audible-Altimeter
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_gpio
        hardware_clocks
        pico_stdlib
        BMP3_SensorAPI::BMP3_SensorAPI
        )
    pico_generate_pio_header(${LIB_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/i2s.pio)

    target_include_directories(${LIB_NAME} PUBLIC ${include_dir})

    target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Audible-Altimeter/include)
else()
    add_library(${LIB_NAME} INTERFACE)
    add_library(${LIB_NAME}::Audible-Altimeter-Embedded ALIAS ${LIB_NAME})
    target_include_directories(${LIB_NAME} INTERFACE ${include_dir})
endif()