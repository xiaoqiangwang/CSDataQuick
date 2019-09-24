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

add_library(UIParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(UIParser PRIVATE
    ${CMAKE_SOURCE_DIR}/src/libs/parser
)

target_compile_definitions(UIParser PRIVATE
    QT_UIC # undef QDESIGNER_UILIB_EXPORT macro in ui4.h
)

target_link_libraries(UIParser PRIVATE
    Qt5::Core parser
)

set_target_properties(UIParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/parser
)

if (APPLE)
    set_target_properties(UIParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS UIParser DESTINATION plugins/parser)
