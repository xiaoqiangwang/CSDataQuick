set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

add_definitions(-DQCS_EXPORT)

include_directories(
    ${CMAKE_BINARY_DIR}/src/include
)

add_library(ADLParser SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(ADLParser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS ADLParser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
