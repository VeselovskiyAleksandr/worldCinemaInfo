cmake_minimum_required (VERSION 3.8)
project (Test)
include(FetchContent)
set(gtest_force_shared_crt ON)  #заставл€ет GoogleTest динамически св€зывать среды выполнени€ и соответствовать проекту, в который он включЄн. 
FetchContent_Declare(
googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG main
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable (Test
                    "../worldCinemaInfoSearchEngine/include/serverTest.cpp"
                    "../worldCinemaInfoSearchEngine/src/InvertedIndex.cpp"
                    "../worldCinemaInfoSearchEngine/src/EngineStart.cpp"
                    "../worldCinemaInfoSearchEngine/src/RequestsJSON.cpp"
                    "../worldCinemaInfoSearchEngine/src/AnswersJSON.cpp"
                     "../worldCinemaInfoSearchEngine/src/ConverterJSON.cpp"
                      "../worldCinemaInfoSearchEngine/src/SearchServer.cpp"
                 )
target_include_directories(Test PRIVATE "../worldCinemaInfoSearchEngine/include/")
target_link_libraries(Test PRIVATE gtest_main nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(Test)