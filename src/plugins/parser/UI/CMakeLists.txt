set(SOURCES
    ui4.cpp
    ui_parser.cpp
    UIParser.cpp
)

set(HEADERS
    ui4.h
    ui_parser.h
    UIParser.h
)

# undef QDESIGNER_UILIB_EXPORT macro in ui4.h
add_definitions(-DQT_UIC)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(UIParser MODULE STATIC
    ${SOURCES}
    ${HEADERS}
)

add_definitions(-DQT_STATICPLUGIN)

target_link_libraries(UIParser Qt5::Core)
