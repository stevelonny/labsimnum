add_executable (exercise_02_1 ex02_1.cpp library.cpp)
target_link_libraries(exercise_02_1 PRIVATE random fmtlib)

add_executable (exercise_02_2 ex02_2.cpp library.cpp)
target_link_libraries(exercise_02_2 PRIVATE random fmtlib)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})