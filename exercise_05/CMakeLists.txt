add_executable (exercise_05 ex05.cpp library.cpp)
target_link_libraries(exercise_05 PRIVATE blocking random fmt::fmt stdc++fs)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})

install(TARGETS exercise_05 DESTINATION exercise_05)
