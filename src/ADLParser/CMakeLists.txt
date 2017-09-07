set(SOURCES
    element.cpp
    parser.cpp
)

set(HEADERS
    common.h
    element.h
    parser.h
)

include_directories(
    ../csdata/core
)

add_library(ADLParser SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(ADLParser PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
