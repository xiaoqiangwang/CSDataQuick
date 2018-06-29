set(SOURCES
    parser.cpp
    EDLParser.cpp
)

set(HEADERS
    EDLParser.h
    EDLParser_global.h
)

add_definitions(-DEDLPARSER_LIBRARY)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
)

add_library(EDLParser SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(EDLParser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS EDLParser
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
