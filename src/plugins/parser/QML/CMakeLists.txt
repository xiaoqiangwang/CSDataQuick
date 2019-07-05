set(SOURCES
    QMLParser.cpp
)

set(HEADERS
    QMLParser.h
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(QMLParser MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(QMLParser Qt5::Core)
