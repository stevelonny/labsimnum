add_executable (exercise_09 ex09.cpp)
target_link_libraries(exercise_09 PRIVATE battle_royale random fmt::fmt stdc++fs)

add_library(
    battle_royale
    battle_royale.h
    battle_royale.cpp
)

target_include_directories(battle_royale PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(battle_royale PRIVATE armadillo random fmt::fmt stdc++fs)

set(CMAKE_CXX_FLAGS "-Wall -O3 -larmadillo")

# file(MAKE_DIRECTORY OUTPUT/CONFIG)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})