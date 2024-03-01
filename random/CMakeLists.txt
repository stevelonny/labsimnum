add_library(
    random
    random.h
    random.cpp
)

target_include_directories(random PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
