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

target_compile_definitions(UIParser PRIVATE
    QT_UIC # undef QDESIGNER_UILIB_EXPORT macro in ui4.h
)

target_compile_options(UIParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4390>
)

target_link_libraries(UIParser PRIVATE
    Qt5::Core csparser
)

set_target_properties(UIParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csparser
)

if (APPLE)
    set_target_properties(UIParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS UIParser DESTINATION ${KDE_INSTALL_PLUGINDIR}/csparser)
