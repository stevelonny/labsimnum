add_executable (exercise_03 ex03.cpp)
target_link_libraries(exercise_03 PRIVATE library03 random fmt::fmt stdc++fs)

add_library(
    library03
    library03.cpp
    library03.h
)

target_include_directories(library03 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(library03 PUBLIC library01 PRIVATE random fmt::fmt stdc++fs)

add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})

install(TARGETS exercise_03 DESTINATION exercise_03)
install(TARGETS library03 DESTINATION exercise_03/lib)
