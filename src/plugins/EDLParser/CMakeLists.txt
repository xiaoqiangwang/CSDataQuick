set(SOURCES
    edl_parser.cpp
    EDLParser.cpp
)

set(HEADERS
    EDLParser.h
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(EDLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(EDLParser Qt5::Core)
target_link_libraries(EDLParser parser)

set_target_properties(EDLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(EDLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS EDLParser DESTINATION plugins/parser)
