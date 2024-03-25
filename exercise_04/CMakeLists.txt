add_executable (exercise_04_eq ex04_eq.cpp)
target_link_libraries(exercise_04_eq PRIVATE random fmt::fmt system particle stdc++fs)

set(CMAKE_CXX_FLAGS "-Wall -O3 -fopenmp -larmadillo")

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})
