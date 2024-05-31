cmake_minimum_required(VERSION 3.13)
project(
    Lab_Simulazione_Numerica
    DESCRIPTION "Laboratorio di Simulazione Numerica di steve"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_FLAGS "-Wall -O3")
set(MAKEFLAGS += -s)

# setting dependencies
include(FetchContent)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmtlib)

find_package(Armadillo REQUIRED)

find_package(OpenMP REQUIRED)

set(MPI_HOME /usr/lib64/mpich/)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
# setting directories
# data directory
file(MAKE_DIRECTORY data)
# dirs to stash NSL simulator IO files
file(MAKE_DIRECTORY input-output/INPUT/CONFIG)
file(MAKE_DIRECTORY input-output/OUTPUT/CONFIG)
# ex04 IO dirs
file(MAKE_DIRECTORY data/ex04/INPUT_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex04/INPUT_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex04/INPUT_LIQUID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_LIQUID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_EQ_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_EQ_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex04/OUTPUT_EQ_LIQUID/CONFIG)
# ex07 IO dirs
file(MAKE_DIRECTORY data/ex07/OUTPUT_NVE_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex07/OUTPUT_NVE_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex07/OUTPUT_NVE_LIQUID/CONFIG)
file(MAKE_DIRECTORY data/ex07/OUTPUT_NVT_SOLID/CONFIG)
file(MAKE_DIRECTORY data/ex07/OUTPUT_NVT_GAS/CONFIG)
file(MAKE_DIRECTORY data/ex07/OUTPUT_NVT_LIQUID/CONFIG)

add_subdirectory(lib)
add_subdirectory(exercise_01)
add_subdirectory(exercise_02)
add_subdirectory(exercise_03)
add_subdirectory(exercise_04)
add_subdirectory(exercise_05)
add_subdirectory(exercise_06)
add_subdirectory(exercise_07)
add_subdirectory(exercise_08)
add_subdirectory(exercise_09)
add_subdirectory(exercise_10)

# setting paths at compile/config time
set(ROOT_PATH "${CMAKE_SOURCE_DIR}")
set(DATA_PATH "${CMAKE_SOURCE_DIR}/data")
set(SEED_PATH "${CMAKE_SOURCE_DIR}/lib/seed.in")
set(PRIMES_PATH "${CMAKE_SOURCE_DIR}/lib/Primes")
set(IO_PATH "${CMAKE_SOURCE_DIR}/input-output")


configure_file( ${CMAKE_SOURCE_DIR}/version_config.hpp.in ${CMAKE_BINARY_DIR}/version_config.h)

# set the install time paths

# set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
# set(ROOT_PATH "${CMAKE_INSTALL_PREFIX}")
# set(DATA_PATH "${CMAKE_INSTALL_PREFIX}/data")
# set(SEED_PATH "${CMAKE_INSTALL_PREFIX}/lib/seed.in")
# set(PRIMES_PATH "${CMAKE_INSTALL_PREFIX}/lib/Primes")
# set(IO_PATH "${CMAKE_INSTALL_PREFIX}/input-output")

