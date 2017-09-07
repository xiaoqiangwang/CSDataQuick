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

if (WIN32)
    set_target_properties(ADLParser PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else ()
    set_target_properties(ADLParser PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()
