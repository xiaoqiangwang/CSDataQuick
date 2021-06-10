set(SOURCES
    edl_parser.cpp
    EDLParser.cpp
)

set(HEADERS
    EDLParser.h
)

add_library(EDLParser STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(EDLParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    QT_STATICPLUGIN
)

target_include_directories(EDLParser PRIVATE
    $<TARGET_PROPERTY:csparser,INCLUDE_DIRECTORIES>
)

target_link_libraries(EDLParser PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
