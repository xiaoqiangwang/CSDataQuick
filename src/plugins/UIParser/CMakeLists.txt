set(SOURCES
    ui4.cpp
    ui_parser.cpp
    UIParser.cpp
)

set(HEADERS
    ui4.h
    UIParser.h
)

# undef QDESIGNER_UILIB_EXPORT macro in ui4.h
add_definitions(-DQT_UIC)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

add_library(UIParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(UIParser Qt5::Core)
target_link_libraries(UIParser parser)

set_target_properties(UIParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(UIParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS UIParser DESTINATION plugins/parser)
