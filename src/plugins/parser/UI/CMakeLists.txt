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

add_library(UIParser STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(UIParser PRIVATE
    QT_UIC # undef QDESIGNER_UILIB_EXPORT macro in ui4.h
    QT_STATICPLUGIN
)

target_compile_options(UIParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4390>
)

target_include_directories(UIParser PRIVATE
    $<TARGET_PROPERTY:csparser,INCLUDE_DIRECTORIES>
)

target_link_libraries(UIParser PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
