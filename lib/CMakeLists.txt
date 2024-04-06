add_library(
    random
    random.h
    random.cpp
)

add_library(
    system
    system.h
    system.cpp
)

add_library(
    particle
    particle.h
    particle.cpp
)

add_library(
    blocking
    blocking.h
    blocking.cpp
)

target_include_directories(random PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(particle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(blocking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(random PRIVATE fmt::fmt stdc++fs)
target_link_libraries(system PRIVATE random fmt::fmt particle stdc++fs)
target_link_libraries(blocking PRIVATE random fmt::fmt stdc++fs)
add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})