set(SOURCES
    element.cpp
    parser.cpp
)

set(HEADERS
    common.h
    element.h
    parser.h
)

# When CMake 3.0.0 is required, remove this block (see CMP0042).
if (NOT DEFINED CMAKEMACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif()

include_directories(
    ../csdata/core
)

add_library(ADLParser SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(ADLParser PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
