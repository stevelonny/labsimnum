add_executable (exercise_04_eq ex04_eq.cpp)
target_link_libraries(exercise_04_eq PRIVATE random fmt::fmt system particle stdc++fs)

add_executable (exercise_04_prep ex04_prep.cpp)
target_link_libraries(exercise_04_prep PRIVATE random fmt::fmt system particle stdc++fs)

add_executable (exercise_04_sim ex04_sim.cpp)
target_link_libraries(exercise_04_sim PRIVATE random fmt::fmt system particle stdc++fs)

add_executable (exercise_04_simeq ex04_simeq.cpp)
target_link_libraries(exercise_04_simeq PRIVATE random fmt::fmt system particle stdc++fs)


set(CMAKE_CXX_FLAGS "-Wall -O3 -fopenmp -larmadillo")

file(MAKE_DIRECTORY OUTPUT/CONFIG)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})

install(TARGETS exercise_04_eq exercise_04_prep exercise_04_sim exercise_04_simeq DESTINATION exercise_04)
install(DIRECTORY OUTPUT/CONFIG DESTINATION exercise_04)
