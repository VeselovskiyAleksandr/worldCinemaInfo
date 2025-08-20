# CMakeList.txt: проект CMake для worldCinemaInfo; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (worldCinemaInfo "src/worldCinemaInfo.cpp"  "include/Configuration.h" "include/ConverterJSON.h" "include/Entry.h" "src/ConverterJSON.cpp" "include/EngineStart.h" "src/EngineStart.cpp"  "include/RequestsJSON.h" "src/RequestsJSON.cpp" "include/AnswersJSON.h" "src/AnswersJSON.cpp")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(worldCinemaInfo PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(worldCinemaInfo PUBLIC include)