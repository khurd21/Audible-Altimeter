set(LIB_NAME Audible-Altimeter)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(sources
    altimeter_data.cpp
    runner.cpp
    audio_player.cpp
    )

set(includes
    ${include_dir}/Audible-Altimeter/altimeter_data.hpp
    ${include_dir}/Audible-Altimeter/runner.hpp
    ${include_dir}/Audible-Altimeter/device_description_interface.hpp
    ${include_dir}/Audible-Altimeter/barometric_sensor_interface.hpp
    ${include_dir}/Audible-Altimeter/timer_interface.hpp
    ${include_dir}/Audible-Altimeter/audio_driver_interface.hpp
    ${include_dir}/Audible-Altimeter/audio_player.hpp
    )

set(AUDIO_INPUT_DIR ${PROJECT_SOURCE_DIR}/assets)
set(AUDIO_OUTPUT_DIR ${CMAKE_BINARY_DIR}/generated)

file(GLOB AUDIO_WAV_FILES "${AUDIO_INPUT_DIR}/*.wav")
set(AUDIO_HPP_FILES)
foreach(wav_file IN LISTS AUDIO_WAV_FILES)
    get_filename_component(filename ${wav_file} NAME_WE)
    list(APPEND AUDIO_HPP_FILES "${AUDIO_OUTPUT_DIR}/${filename}.hpp")
endforeach()

file(MAKE_DIRECTORY ${AUDIO_OUTPUT_DIR})
add_custom_command(
    OUTPUT ${AUDIO_HPP_FILES}
    COMMAND python3 ${PROJECT_SOURCE_DIR}/scripts/convert_audio_to_code.py
            ${AUDIO_INPUT_DIR} ${AUDIO_OUTPUT_DIR}
    DEPENDS ${AUDIO_WAV_FILES}
    COMMENT "Converting .wav files to .hpp files"
)

add_custom_target(generate_audio_files ALL
    DEPENDS ${AUDIO_HPP_FILES}
)

add_library(${LIB_NAME} STATIC ${sources} ${includes} ${AUDIO_HPP_FILES})
add_library(${LIB_NAME}::Audible-Altimeter ALIAS ${LIB_NAME})
add_dependencies(${LIB_NAME} generate_audio_files)

target_include_directories(${LIB_NAME} PUBLIC ${include_dir})
target_include_directories(${LIB_NAME} PUBLIC ${AUDIO_OUTPUT_DIR})
