add_library(
    fmtlib
    fmtlib.h
    ostream.h
)

set_target_properties(fmtlib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(fmtlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})