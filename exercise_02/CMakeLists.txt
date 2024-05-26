add_executable (exercise_02_1 ex02_1.cpp)
target_link_libraries(exercise_02_1 PRIVATE library02 random fmt::fmt stdc++fs)

add_executable (exercise_02_2 ex02_2.cpp)
target_link_libraries(exercise_02_2 PRIVATE library02 random fmt::fmt stdc++fs)

add_library(
    library02
    library02.cpp
    library02.h
)

target_include_directories(library02 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(library02 PUBLIC library01 PRIVATE random fmt::fmt stdc++fs)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})