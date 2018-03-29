set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
    ADLParser_global.h
)

add_definitions(-DADLPARSER_LIBRARY)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
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
