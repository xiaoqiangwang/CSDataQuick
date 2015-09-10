set(SOURCES
    fileinfo.cpp
    common.cpp
    element.cpp
    parser.cpp
)

set(HEADERS
    fileinfo.h
    common.h
    element.h
    parser.h
)

add_library(ADLParser STATIC
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(ADLParser PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
