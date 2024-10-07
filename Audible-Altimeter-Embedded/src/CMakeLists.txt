set(LIB_NAME Audible-Altimeter-Embedded)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(sources
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
    )

if (${IS_PICO})
    add_library(${LIB_NAME} STATIC ${sources} ${includes})
    add_library(${LIB_NAME}::Audible-Altimeter-Embedded ALIAS ${LIB_NAME})
    target_link_libraries(${LIB_NAME} PUBLIC
        hardware_i2c
        pico_stdlib
        BMP3_SensorAPI::BMP3_SensorAPI
        )

    target_include_directories(${LIB_NAME} PUBLIC ${include_dir})
    target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../Audible-Altimeter/include)
else()
    add_library(${LIB_NAME} INTERFACE)
    add_library(${LIB_NAME}::Audible-Altimeter-Embedded ALIAS ${LIB_NAME})
    target_include_directories(${LIB_NAME} INTERFACE ${include_dir})
endif()