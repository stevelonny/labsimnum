add_executable (exercise_06 ex06.cpp)
target_link_libraries(exercise_06 PRIVATE random fmt::fmt system particle stdc++fs)

set(CMAKE_CXX_FLAGS "-Wall -O3 -larmadillo")

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})

install(TARGETS exercise_06)
