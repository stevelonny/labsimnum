cmake_minimum_required(VERSION 3.13)
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

find_package(OpenMP REQUIRED)

#setting directories
file(MAKE_DIRECTORY data)
file(MAKE_DIRECTORY input-output/INPUT/CONFIG)
file(MAKE_DIRECTORY input-output/OUTPUT/CONFIG)
file(MAKE_DIRECTORY data/ex04/INPUT_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex04/INPUT_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex04/INPUT_LIQUID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_LIQUID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_EQ_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_EQ_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_EQ_LIQUID/CONFIG)


add_subdirectory(lib)
add_subdirectory(exercise_01)
add_subdirectory(exercise_02)
add_subdirectory(exercise_03)
add_subdirectory(exercise_04)
add_subdirectory(exercise_05)

configure_file( ${CMAKE_SOURCE_DIR}/version_config.hpp.in ${CMAKE_BINARY_DIR}/version_config.h)

