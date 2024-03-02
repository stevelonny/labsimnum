add_executable (exercise_01 ex01_1.cpp library.cpp)
target_link_libraries(exercise_01 PRIVATE random fmtlib)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})