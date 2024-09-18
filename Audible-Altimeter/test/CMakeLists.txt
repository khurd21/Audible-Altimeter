if (NOT ${IS_PICO})

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY git@github.com:google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)
# find_package(GTest REQUIRED)
add_executable(Audible-Altimeter-Tests
  dummy_test.cpp
)

target_link_libraries(Audible-Altimeter-Tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Audible-Altimeter-Tests)

endif()