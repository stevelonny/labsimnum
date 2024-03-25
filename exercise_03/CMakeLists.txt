add_executable (exercise_03 ex03.cpp library.cpp)
target_link_libraries(exercise_03 PRIVATE random fmt::fmt stdc++fs)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})