cmake_minimum_required(VERSION 3.11)
project(
    Lab_Simulazione_Numerica
    DESCRIPTION "Laboratorio di Simulazione Numerica di steve"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_FLAGS "-Wall -O3")
set(MAKEFLAGS += -s)

#setting dependencies
include(FetchContent)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmtlib)

find_package(Armadillo REQUIRED)

#setting directories
add_subdirectory(random)
add_subdirectory(exercise_01)
add_subdirectory(exercise_02)
add_subdirectory(exercise_03)

configure_file( ${CMAKE_SOURCE_DIR}/version_config.hpp.in ${CMAKE_BINARY_DIR}/version_config.h)

