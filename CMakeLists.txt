cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(Lab_Simulazione_Numerica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_FLAGS "-Wall -O3")
set(MAKEFLAGS += -s)

add_subdirectory(random)
add_subdirectory(fmt)
add_subdirectory(exercise_01)
add_subdirectory(exercise_02)

configure_file( ${CMAKE_SOURCE_DIR}/version_config.hpp.in ${CMAKE_BINARY_DIR}/version_config.h)
