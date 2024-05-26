add_executable (exercise_01_1 ex01_1.cpp)
target_link_libraries(exercise_01_1 PRIVATE library01 random fmt::fmt stdc++fs)

add_executable (exercise_01_2 ex01_2.cpp)
target_link_libraries(exercise_01_2 PRIVATE library01 random fmt::fmt stdc++fs)

add_executable (exercise_01_3 ex01_3.cpp)
target_link_libraries(exercise_01_3 PRIVATE library01 random fmt::fmt stdc++fs)

add_library(
    library01
    library01.cpp
    library01.h
)

target_include_directories(library01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(library01 PRIVATE fmt::fmt stdc++fs random)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})