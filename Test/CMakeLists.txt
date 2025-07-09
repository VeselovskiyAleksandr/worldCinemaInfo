cmake_minimum_required (VERSION 3.8)
project (Test)
include(FetchContent)

FetchContent_Declare(
googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG main
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable (Test
                    "../worldCinemaInfo/include/serverTest.cpp"
                    "../worldCinemaInfo/src/ConverterJSON.cpp"
               )
target_include_directories(Test PRIVATE "../worldCinemaInfo/include/")
target_link_libraries(Test PRIVATE gtest_main nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(Test)