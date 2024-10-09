set(LIB_NAME Audible-Altimeter)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(sources
    altimeter_data.cpp
    runner.cpp
    )

set(includes
    ${include_dir}/Audible-Altimeter/altimeter_data.hpp
    ${include_dir}/Audible-Altimeter/runner.hpp
    ${include_dir}/Audible-Altimeter/device_description_interface.hpp
    ${include_dir}/Audible-Altimeter/barometric_sensor_interface.hpp
    ${include_dir}/Audible-Altimeter/timer_interface.hpp
    )

add_library(${LIB_NAME} STATIC ${sources} ${includes})
add_library(${LIB_NAME}::Audible-Altimeter ALIAS ${LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC ${include_dir})
