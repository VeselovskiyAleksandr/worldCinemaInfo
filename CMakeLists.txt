﻿# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project ("worldCinemaInfo")

# Включите подпроекты.
add_subdirectory ("worldCinemaInfo")
add_subdirectory(Test)
add_subdirectory(nlohmann.json)
add_subdirectory("environmentVariables")
