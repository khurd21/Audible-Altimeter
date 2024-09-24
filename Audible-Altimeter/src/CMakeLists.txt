set(LIB_NAME Audible-Altimeter)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(sources
    Runner.cpp
    )

set(includes
    ${include_dir}/Audible-Altimeter/Runner.hpp
    )

add_library(${LIB_NAME} STATIC ${sources} ${includes})
add_library(${LIB_NAME}::Audible-Altimeter ALIAS ${LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC ${include_dir})

# TODO: Make the embedded library
target_link_libraries(${LIB_NAME} PRIVATE Audible-Altimeter-Embedded::Audible-Altimeter-Embedded)
