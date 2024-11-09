if (NOT ${IS_PICO})

include(FetchContent)
enable_testing()

find_package(GTest QUIET)

if(NOT GTest_FOUND)
  message(STATUS "GTest not found. Pulling from repository.")
  FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.15.2
  )
  FetchContent_MakeAvailable(googletest)
endif()

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(data
  data/barometric_sensor_test_data.hpp
  data/runner_test_data.hpp

  data/temperature_test_data.json
  data/pressure_test_data.json
  data/runner_test_data.json)

set(mocks
  mocks/audio_driver_interface.hpp
  mocks/barometric_sensor_interface.hpp
  mocks/device_description_interface.hpp
  mocks/timer_interface.hpp)

set(tests
  altimeter_data_tests.cpp
  runner_tests.cpp)
  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(DATA_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/data")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/data/path_to_data_folder.hpp.in
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/path_to_data_folder.hpp
  @ONLY
)

add_executable(Audible-Altimeter-Tests
  ${data}
  ${mocks}
  ${tests}
)

target_include_directories(Audible-Altimeter-Tests PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_link_libraries(Audible-Altimeter-Tests GTest::gtest_main GTest::gmock)
target_link_libraries(Audible-Altimeter-Tests Audible-Altimeter::Audible-Altimeter)
target_link_libraries(Audible-Altimeter-Tests nlohmann_json::nlohmann_json)


include(GoogleTest)
gtest_discover_tests(Audible-Altimeter-Tests)

endif()