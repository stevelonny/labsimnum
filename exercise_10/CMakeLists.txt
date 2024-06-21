add_executable(exercise_10 ex10.cpp)
target_compile_definitions(exercise_10 PRIVATE WITH_MPI)
target_include_directories(exercise_10 PRIVATE ${MPI_CXX_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(exercise_10 PRIVATE MPI::MPI_CXX battle_royale random fmt::fmt stdc++fs)

add_executable(exercise_10_2 ex10_2.cpp)
target_compile_definitions(exercise_10_2 PRIVATE WITH_MPI)
target_include_directories(exercise_10_2 PRIVATE ${MPI_CXX_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(exercise_10_2 PRIVATE MPI::MPI_CXX battle_royale random fmt::fmt stdc++fs)

add_executable(trials trials.cpp)
target_compile_definitions(trials PRIVATE WITH_MPI)
target_include_directories(trials PRIVATE ${MPI_CXX_INCLUDE_DIRS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(trials PRIVATE MPI::MPI_CXX random fmt::fmt stdc++fs)

set(CMAKE_CXX_FLAGS "-Wall -O3 -larmadillo")

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR} ${MPI_CXX_INCLUDE_DIRS})
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

