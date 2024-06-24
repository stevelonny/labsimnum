add_executable (exercise_08_1 ex08_1.cpp library08.cpp)
target_link_libraries(exercise_08_1 PRIVATE blocking random fmt::fmt stdc++fs)

add_executable (exercise_08_2 ex08_2.cpp library08.cpp)
target_link_libraries(exercise_08_2 PRIVATE blocking random fmt::fmt stdc++fs)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})

install(TARGETS exercise_08_1 exercise_08_2)
