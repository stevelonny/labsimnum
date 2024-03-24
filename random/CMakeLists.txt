add_library(
    random
    random.h
    random.cpp
)

target_include_directories(random PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(random PRIVATE fmt::fmt)
add_definitions(-D_VAR=\"${CMAKE_SOURCE_DIR}\")
include_directories( ${CMAKE_BINARY_DIR})