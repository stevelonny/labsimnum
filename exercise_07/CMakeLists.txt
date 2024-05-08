add_executable (exercise_07_acc ex07_acc.cpp)
target_link_libraries(exercise_07_acc PRIVATE random fmt::fmt system particle stdc++fs)

add_executable (exercise_07_2 ex07_2.cpp)
target_link_libraries(exercise_07_2 PRIVATE random fmt::fmt system particle stdc++fs)

add_executable (exercise_07_4 ex07_4.cpp)
target_link_libraries(exercise_07_4 PRIVATE random fmt::fmt system particle stdc++fs)

add_executable (acceptance NSL_SIMULATOR.cpp)
target_link_libraries(acceptance PRIVATE random fmt::fmt system particle stdc++fs)

set(CMAKE_CXX_FLAGS "-Wall -O3 -larmadillo -fopenmp")

file(MAKE_DIRECTORY OUTPUT/CONFIG)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})