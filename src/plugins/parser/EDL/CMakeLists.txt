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

add_library(EDLParser MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(EDLParser Qt5::Core)
