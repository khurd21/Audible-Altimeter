if (NOT ${IS_PICO})

enable_testing()

find_package(GTest QUIET)

if(NOT GTest_FOUND)
  message(STATUS "GTest not found. Pulling from repository.")
  include(FetchContent)
  FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.15.2
  )
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(Audible-Altimeter-Tests
  dummy_test.cpp
)

target_link_libraries(Audible-Altimeter-Tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Audible-Altimeter-Tests)

endif()