set(LIB_NAME Audible-Altimeter-Embedded)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(sources
    dummy.cpp
    )
set(includes
    ${include_dir}/Audible-Altimeter-Embedded/dummy.hpp
    )

if (${IS_PICO})
    add_library(${LIB_NAME} STATIC ${sources} ${includes})
    add_library(${LIB_NAME}::Audible-Altimeter-Embedded ALIAS ${LIB_NAME})
    target_include_directories(${LIB_NAME} PUBLIC ${include_dir})
    target_link_libraries(${LIB_NAME} PRIVATE
        hardware_i2c
        pico_stdlib
        BMP3_SensorAPI::BMP3_SensorAPI
        )
else()
    add_library(${LIB_NAME} INTERFACE)
    add_library(${LIB_NAME}::Audible-Altimeter-Embedded ALIAS ${LIB_NAME})
    target_include_directories(${LIB_NAME} INTERFACE ${include_dir})
endif()