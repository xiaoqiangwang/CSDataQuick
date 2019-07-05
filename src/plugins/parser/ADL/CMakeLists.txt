set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(ADLParser MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(ADLParser Qt5::Core)
