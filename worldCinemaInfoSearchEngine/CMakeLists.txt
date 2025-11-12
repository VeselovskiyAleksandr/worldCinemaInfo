# CMakeList.txt: проект CMake для worldCinemaInfoSearchEngine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (worldCinemaInfoSearchEngine "src/worldCinemaInfoSearchEngine.cpp"  "include/Configuration.h" "include/Entry.h" "src/InvertedIndex.cpp" "include/EngineStart.h" "src/EngineStart.cpp"  "include/RequestsJSON.h" "src/RequestsJSON.cpp" "include/AnswersJSON.h" "src/AnswersJSON.cpp" "include/InvertedIndex.h"  "include/SearchServer.h" "src/SearchServer.cpp" "include/ConverterJSON.h" "src/ConverterJSON.cpp")
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(worldCinemaInfoSearchEngine PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(worldCinemaInfoSearchEngine PUBLIC include)