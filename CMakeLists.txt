cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Lab_Simulazione_Numerica)

set(CMAKE_CXX_FLAGS "-Wall -O3 --std=c++17")
set(MAKEFLAGS += -s)

add_subdirectory(random)
add_subdirectory(exercise_01)

configure_file( ${CMAKE_SOURCE_DIR}/version_config.h.in ${CMAKE_BINARY_DIR}/version_config.h)
